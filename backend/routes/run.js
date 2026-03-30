const fs = require("fs/promises");
const path = require("path");
const { spawn } = require("child_process");
const os = require("os");

const ROOT_DIR = path.resolve(__dirname, "..", "..");
const PARSER_DIR = path.join(ROOT_DIR, "parser");
const LEXER_DIR = path.join(ROOT_DIR, "lexer");
const OUTPUT_C_PATH = path.join(ROOT_DIR, "output", "output.c");
const TEMP_ROOT = path.join(__dirname, "..", "temp");

const PARSER_EXE = process.platform === "win32" ? "wizard_parser.exe" : "wizard_parser";
const LEXER_EXE = process.platform === "win32" ? "wizard_lexer.exe" : "wizard_lexer";
const COMPILED_OUT = process.platform === "win32" ? "program.exe" : "program.out";

const MAX_SOURCE_BYTES = 100_000;
const MAX_RUNTIME_INPUT_BYTES = 20_000;
const PROCESS_TIMEOUT_MS = 7_000;

let runQueue = Promise.resolve();

function classifyErrors(stdErr, stdOut) {
  const all = `${stdErr}\n${stdOut}`;
  const errors = [];

  const semanticRe = /Semantic Error \(line (\d+)\):\s*(.+)/gi;
  const syntaxRe = /Parse error:\s*(.+)/gi;
  const lexicalRe = /Lexical Error(?: \(line (\d+)\))?:\s*(.+)/gi;

  let m;
  while ((m = semanticRe.exec(all)) !== null) {
    errors.push({
      type: "semantic",
      line: Number(m[1]),
      message: `Semantic Error (line ${m[1]}): ${m[2]}`
    });
  }

  while ((m = syntaxRe.exec(all)) !== null) {
    errors.push({
      type: "syntax",
      line: null,
      message: `Syntax Error: ${m[1]}`
    });
  }

  while ((m = lexicalRe.exec(all)) !== null) {
    errors.push({
      type: "lexical",
      line: m[1] ? Number(m[1]) : null,
      message: m[1] ? `Lexical Error (line ${m[1]}): ${m[2]}` : `Lexical Error: ${m[2]}`
    });
  }

  return errors;
}

function extractBetween(text, startTag, endTag) {
  const start = text.indexOf(startTag);
  const end = text.indexOf(endTag);
  if (start === -1 || end === -1 || end <= start) {
    return "";
  }
  return text.slice(start + startTag.length, end).trim();
}

function spawnWithCapture(command, args, options = {}) {
  return new Promise((resolve) => {
    const child = spawn(command, args, {
      cwd: options.cwd,
      env: options.env,
      shell: false,
      windowsHide: true
    });

    let stdout = "";
    let stderr = "";
    let timedOut = false;

    const timer = setTimeout(() => {
      timedOut = true;
      child.kill(process.platform === "win32" ? "SIGTERM" : "SIGKILL");
    }, options.timeoutMs ?? PROCESS_TIMEOUT_MS);

    child.stdout.on("data", (chunk) => {
      stdout += chunk.toString();
    });

    child.stderr.on("data", (chunk) => {
      stderr += chunk.toString();
    });

    child.stdin.on("error", () => {});
    if (typeof options.stdin === "string" && options.stdin.length > 0) {
      child.stdin.write(options.stdin);
    }
    child.stdin.end();

    child.on("close", (code) => {
      clearTimeout(timer);
      resolve({ code, stdout, stderr, timedOut });
    });

    child.on("error", (err) => {
      clearTimeout(timer);
      resolve({ code: -1, stdout, stderr: `${stderr}\n${err.message}`, timedOut });
    });
  });
}

async function compileAndRunC(tempDir, cCode, runtimeInput) {
  if (!cCode || !cCode.trim()) {
    return { output: "", error: "No generated C code found.", success: false };
  }

  const localCPath = path.join(tempDir, "output.c");
  const localExePath = path.join(tempDir, COMPILED_OUT);
  await fs.writeFile(localCPath, cCode, "utf8");

  const compilerCmd = process.platform === "win32" ? "g++" : "gcc";
  const compileRes = await spawnWithCapture(
    compilerCmd,
    [localCPath, "-o", localExePath, "-lm"],
    { cwd: tempDir, timeoutMs: PROCESS_TIMEOUT_MS }
  );

  if (compileRes.timedOut) {
    return {
      output: "",
      error: "C compilation timed out.",
      success: false,
      compileStdErr: compileRes.stderr
    };
  }

  if (compileRes.code !== 0) {
    return {
      output: "",
      error: "Generated C compilation failed.",
      success: false,
      compileStdErr: compileRes.stderr
    };
  }

  const runCmd = process.platform === "win32" ? localExePath : `./${COMPILED_OUT}`;
  const runArgs = process.platform === "win32" ? [] : [];

  const runRes = await spawnWithCapture(runCmd, runArgs, {
    cwd: tempDir,
    stdin: runtimeInput,
    timeoutMs: PROCESS_TIMEOUT_MS
  });

  if (runRes.timedOut) {
    return {
      output: runRes.stdout,
      error: "Program execution timed out.",
      success: false,
      runtimeStdErr: runRes.stderr
    };
  }

  return {
    output: runRes.stdout.trim(),
    error: runRes.code === 0 ? "" : (runRes.stderr || "Program exited with non-zero status."),
    success: runRes.code === 0
  };
}

async function runCompilerPipeline(sourceCode, includeTokens, runtimeInput) {
  await fs.mkdir(TEMP_ROOT, { recursive: true });
  const tempDir = await fs.mkdtemp(path.join(TEMP_ROOT, `run-${Date.now()}-${os.tmpdir().split(path.sep).pop()}-`));

  const inputPath = path.join(tempDir, "program.wiz");
  await fs.writeFile(inputPath, sourceCode, "utf8");

  // Prevent stale reads when a parse fails before code generation.
  await fs.rm(OUTPUT_C_PATH, { force: true });

  let tokens = "";
  if (includeTokens) {
    const lexerPath = path.join(LEXER_DIR, LEXER_EXE);
    const lexerRes = await spawnWithCapture(lexerPath, [inputPath], {
      cwd: ROOT_DIR,
      timeoutMs: PROCESS_TIMEOUT_MS
    });

    tokens = lexerRes.stdout.trim();
    if (lexerRes.code !== 0 && lexerRes.stderr) {
      tokens += `\n${lexerRes.stderr.trim()}`;
    }
  }

  const parserPath = path.join(PARSER_DIR, PARSER_EXE);
  const parserRes = await spawnWithCapture(parserPath, [inputPath], {
    cwd: PARSER_DIR,
    timeoutMs: PROCESS_TIMEOUT_MS
  });

  const parserStdOut = parserRes.stdout || "";
  const parserStdErr = parserRes.stderr || "";
  const parseSuccessful = /Parse successful\./.test(parserStdOut);

  let cCode = "";
  if (parseSuccessful) {
    try {
      cCode = await fs.readFile(OUTPUT_C_PATH, "utf8");
    } catch {
      cCode = "";
    }
  }

  const rawIcg = extractBetween(parserStdOut, "=== RAW_ICG_START ===", "=== RAW_ICG_END ===");
  const optimizedIcg = extractBetween(parserStdOut, "=== OPT_ICG_START ===", "=== OPT_ICG_END ===");

  const errors = classifyErrors(parserStdErr, parserStdOut);

  let execution = {
    output: "",
    error: "",
    success: false
  };

  if (!parserRes.timedOut && parseSuccessful && errors.length === 0 && cCode.trim()) {
    execution = await compileAndRunC(tempDir, cCode, runtimeInput);
  }

  const success = !parserRes.timedOut && parseSuccessful && errors.length === 0;

  await fs.rm(tempDir, { recursive: true, force: true });

  return {
    success,
    parserExitCode: parserRes.code,
    parserTimedOut: parserRes.timedOut,
    parseSuccessful,
    tokens,
    errors,
    intermediate_code: rawIcg,
    optimized_code: optimizedIcg || rawIcg,
    generated_c_code: cCode,
    execution_output: execution.output,
    execution_error: execution.error,
    raw_stdout: parserStdOut.trim(),
    raw_stderr: parserStdErr.trim()
  };
}

async function queueRun(sourceCode, includeTokens, runtimeInput) {
  const next = runQueue.then(() => runCompilerPipeline(sourceCode, includeTokens, runtimeInput));
  runQueue = next.catch(() => {});
  return next;
}

module.exports = function registerRunRoute(app) {
  app.post("/run", async (req, res) => {
    const sourceCode = typeof req.body?.code === "string" ? req.body.code : "";
    const includeTokens = Boolean(req.body?.includeTokens);
    const runtimeInput = typeof req.body?.runtimeInput === "string" ? req.body.runtimeInput : "";

    if (!sourceCode.trim()) {
      return res.status(400).json({
        success: false,
        errors: [{ type: "input", line: null, message: "Input code cannot be empty." }]
      });
    }

    if (Buffer.byteLength(sourceCode, "utf8") > MAX_SOURCE_BYTES) {
      return res.status(413).json({
        success: false,
        errors: [{ type: "input", line: null, message: "Input code is too large." }]
      });
    }

    if (Buffer.byteLength(runtimeInput, "utf8") > MAX_RUNTIME_INPUT_BYTES) {
      return res.status(413).json({
        success: false,
        errors: [{ type: "input", line: null, message: "Runtime input is too large." }]
      });
    }

    try {
      const result = await queueRun(sourceCode, includeTokens, runtimeInput);
      return res.json(result);
    } catch (err) {
      return res.status(500).json({
        success: false,
        errors: [{ type: "server", line: null, message: `Internal server error: ${err.message}` }]
      });
    }
  });
};
