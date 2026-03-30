# Arcane Compiler Backend

This backend exposes the Wizard compiler as a web API.

## Endpoints

- `GET /health` - service heartbeat
- `POST /run` - compile and run Wizard code

## Request (`POST /run`)

```json
{
  "code": "ENTER_HOGWARTS\nHOUSE Gryffindor\nDECLARE x AS INT := 42;\nCAST x;\nENDHOUSE\nEXIT_HOGWARTS\n",
  "includeTokens": true
}
```

## Response (example)

```json
{
  "success": true,
  "errors": [],
  "tokens": "...",
  "intermediate_code": "...",
  "optimized_code": "...",
  "generated_c_code": "...",
  "execution_output": "42"
}
```

## Security notes

- No shell command execution (`spawn` with argument array only)
- Request size limit enforced
- Timeout enforced for compiler, C compile, and runtime execution
- Per-request temp directory cleanup
- Serialized compile runs to avoid race on shared compiler output artifacts

## Run locally

From project root:

```powershell
cd backend
npm install
npm start
```

Then open:

- `http://localhost:4000/` (frontend + API)
- `http://localhost:4000/health`
