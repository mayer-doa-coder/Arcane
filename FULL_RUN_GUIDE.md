# Arcane Full Run Guide (Windows)

This guide shows the exact order to run the full pipeline:

Wizard source (`.wiz`) → Parser/ICG/Optimizer/Codegen → `output/output.c` → native `.exe`

---

## 1) Where to write your Wizard program

Write your source file inside:

- `d:\Arcane\input\`

Example file:

- `d:\Arcane\input\my_program.wiz`

---

## 2) PowerShell steps (recommended)

### Step A — go to parser folder

```powershell
Set-Location d:\Arcane\parser
```

### Step B — set bison/m4 environment (for your setup)

```powershell
$env:BISON_PKGDATADIR='C:\PROGRA~2\GnuWin32\share\bison'
$env:M4='C:\PROGRA~2\GnuWin32\bin\m4.exe'
```

### Step C — build parser + all modules

```powershell
bison -d wizard.y
g++ -Wall wizard.tab.c ..\symbol_table\symbol_table.c ..\icg\icg.c ..\optimizer\optimizer.c ..\codegen\codegen.c -o wizard_parser.exe
```

### Step D — run your Wizard source (this generates IR + `output/output.c`)

```powershell
.\wizard_parser.exe ..\input\my_program.wiz
```

### Step E — compile generated C program

```powershell
g++ -Wall ..\output\output.c -o ..\output\wizard_from_c.exe
```

### Step F — run generated executable

```powershell
..\output\wizard_from_c.exe
```

---

## 3) CMD steps (alternative)

### Step A — go to parser folder

```cmd
cd /d d:\Arcane\parser
```

### Step B — set bison/m4 environment

```cmd
set BISON_PKGDATADIR=C:\PROGRA~2\GnuWin32\share\bison
set M4=C:\PROGRA~2\GnuWin32\bin\m4.exe
```

### Step C — build parser + all modules

```cmd
bison -d wizard.y
g++ -Wall wizard.tab.c ..\symbol_table\symbol_table.c ..\icg\icg.c ..\optimizer\optimizer.c ..\codegen\codegen.c -o wizard_parser.exe
```

### Step D — run Wizard source

```cmd
wizard_parser.exe ..\input\my_program.wiz
```

### Step E — compile generated C

```cmd
g++ -Wall ..\output\output.c -o ..\output\wizard_from_c.exe
```

### Step F — run generated executable

```cmd
..\output\wizard_from_c.exe
```

---

## 4) Files you should expect

After Step D:

- `d:\Arcane\output\output.c`

After Step E:

- `d:\Arcane\output\wizard_from_c.exe`

---

## 5) Quick troubleshooting

- If `bison` fails with `m4` path errors, re-check Step B env vars.
- If `output/output.c` is missing, Step D did not complete successfully.
- If C compile fails in Step E, open and inspect `d:\Arcane\output\output.c`.
