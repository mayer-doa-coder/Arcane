param(
    [string]$OutDir = "grading_artifacts",
    [switch]$SkipBuild
)

$ErrorActionPreference = "Continue"
$repoRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$parserDir = Join-Path $repoRoot "parser"
$inputDir = Join-Path $repoRoot "input"
$outputDir = Join-Path $repoRoot "output"
$lexerExe = Join-Path $repoRoot "lexer\wizard_lexer.exe"

$timestamp = Get-Date -Format "yyyy-MM-dd_HHmmss"
$runRoot = Join-Path $repoRoot (Join-Path $OutDir $timestamp)
$logsDir = Join-Path $runRoot "logs"
$generatedDir = Join-Path $runRoot "generated_c"
$exeDir = Join-Path $runRoot "exe"

New-Item -ItemType Directory -Path $logsDir -Force | Out-Null
New-Item -ItemType Directory -Path $generatedDir -Force | Out-Null
New-Item -ItemType Directory -Path $exeDir -Force | Out-Null

$tests = @(
    @{ Name = "grade_valid_simple_decl_print"; Kind = "valid"; CompileC = $true },
    @{ Name = "grade_valid_medium_condition_assign"; Kind = "valid"; CompileC = $true },
    @{ Name = "grade_valid_complex_loop_function_output"; Kind = "valid"; CompileC = $true },

    @{ Name = "grade_negative_invalid_token"; Kind = "negative"; CompileC = $false },
    @{ Name = "grade_negative_syntax_missing_endcheck"; Kind = "negative"; CompileC = $false },
    @{ Name = "grade_negative_semantic_undeclared_var"; Kind = "negative"; CompileC = $false },
    @{ Name = "grade_negative_semantic_duplicate_decl"; Kind = "negative"; CompileC = $false },
    @{ Name = "grade_negative_semantic_type_mismatch"; Kind = "negative"; CompileC = $false },
    @{ Name = "grade_negative_semantic_invalid_condition"; Kind = "negative"; CompileC = $false },
    @{ Name = "grade_negative_semantic_wrong_function_args"; Kind = "negative"; CompileC = $false },

    @{ Name = "grade_edge_empty_program"; Kind = "edge"; CompileC = $true },
    @{ Name = "grade_edge_nested_structures"; Kind = "edge"; CompileC = $true },
    @{ Name = "grade_edge_multiple_errors"; Kind = "edge"; CompileC = $false },
    @{ Name = "grade_edge_break_continue_outside_loop"; Kind = "edge"; CompileC = $false },
    @{ Name = "grade_edge_call_before_declaration"; Kind = "edge"; CompileC = $false }
)

Push-Location $parserDir
try {
    if (-not $SkipBuild) {
        Write-Host ">>> Building parser"
        bison -d wizard.y
        if ($LASTEXITCODE -ne 0) { throw "bison failed" }

        g++ -Wall wizard.tab.c ..\symbol_table\symbol_table.c ..\icg\icg.c ..\optimizer\optimizer.c ..\codegen\codegen.c -o wizard_parser.exe
        if ($LASTEXITCODE -ne 0) { throw "g++ build failed" }
    }

    $summary = @()

    foreach ($t in $tests) {
        $testFile = Join-Path $inputDir ("{0}.wiz" -f $t.Name)
        $logFile = Join-Path $logsDir ("{0}.log" -f $t.Name)
        $cCopy = Join-Path $generatedDir ("{0}.c" -f $t.Name)
        $exePath = Join-Path $exeDir ("{0}.exe" -f $t.Name)
        $runtimeLog = Join-Path $logsDir ("{0}.runtime.log" -f $t.Name)
        $lexerLog = Join-Path $logsDir ("{0}.lexer.log" -f $t.Name)
        $lexerTokenOut = Join-Path $logsDir ("{0}.tokens.txt" -f $t.Name)

        Write-Host (">>> Running {0}" -f $t.Name)
        $parserCmd = '".\wizard_parser.exe" "{0}" > "{1}" 2>&1' -f $testFile, $logFile
        cmd /c $parserCmd
        $parserExit = $LASTEXITCODE

        if (Test-Path (Join-Path $outputDir "output.c")) {
            Copy-Item (Join-Path $outputDir "output.c") $cCopy -Force
        }

        $compiled = $false
        $runtimeExit = $null
        $generatedCPath = ""
        $runtimeLogPath = ""
        $lexerLogPath = ""

        if (Test-Path $cCopy) {
            $generatedCPath = (Resolve-Path $cCopy).Path
        }

        if ($t.CompileC -and (Test-Path $cCopy)) {
            g++ -Wall $cCopy -o $exePath 2>> $logFile
            if ($LASTEXITCODE -eq 0) {
                $compiled = $true
                $runtimeCmd = '"{0}" > "{1}" 2>&1' -f $exePath, $runtimeLog
                cmd /c $runtimeCmd
                $runtimeExit = $LASTEXITCODE
                if (Test-Path $runtimeLog) {
                    $runtimeLogPath = (Resolve-Path $runtimeLog).Path
                }
            }
        }

        if ($t.Name -eq "grade_negative_invalid_token" -and (Test-Path $lexerExe)) {
            $lexerCmd = '"{0}" "{1}" "{2}" > "{3}" 2>&1' -f $lexerExe, $testFile, $lexerTokenOut, $lexerLog
            cmd /c $lexerCmd | Out-Null
            if (Test-Path $lexerLog) {
                $lexerLogPath = (Resolve-Path $lexerLog).Path
            }
        }

        $hasLexicalError = $false
        $hasParseError = $false
        $hasSemanticError = $false
        if (Test-Path $logFile) {
            $raw = Get-Content -Path $logFile -Raw
            if ($raw -match "Lexical Error") { $hasLexicalError = $true }
            if ($raw -match "Parse error") { $hasParseError = $true }
            if ($raw -match "Semantic Error") { $hasSemanticError = $true }
        }
        if ((-not $hasLexicalError) -and (Test-Path $lexerLog)) {
            $rawLexer = Get-Content -Path $lexerLog -Raw
            if ($rawLexer -match "Lexical Error") { $hasLexicalError = $true }
        }

        $summary += [PSCustomObject]@{
            Test = $t.Name
            Category = $t.Kind
            ParserExit = $parserExit
            CCompiled = $compiled
            RuntimeExit = $runtimeExit
            HasLexicalError = $hasLexicalError
            HasParseError = $hasParseError
            HasSemanticError = $hasSemanticError
            ParserLog = (Resolve-Path $logFile).Path
            LexerLog = $lexerLogPath
            GeneratedC = $generatedCPath
            RuntimeLog = $runtimeLogPath
        }
    }

    $csvPath = Join-Path $runRoot "summary.csv"
    $txtPath = Join-Path $runRoot "summary.txt"
    $summary | Export-Csv -NoTypeInformation -Path $csvPath
    $summary | Format-Table -AutoSize | Out-String | Set-Content -Path $txtPath

    Write-Host ""
    Write-Host "Artifacts written to: $runRoot"
    Write-Host "Summary: $csvPath"
}
finally {
    Pop-Location
}
