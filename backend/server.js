const express = require("express");
const cors = require("cors");
const path = require("path");
const fs = require("fs");

const registerRunRoute = require("./routes/run");

const app = express();
const PORT = process.env.PORT || 4000;

app.use(cors());
app.use(express.json({ limit: "1mb" }));

app.get("/health", (_req, res) => {
  res.json({
    ok: true,
    service: "arcane-compiler-service",
    timestamp: new Date().toISOString()
  });
});

registerRunRoute(app);

const rootDir = path.resolve(__dirname, "..");
const indexPath = path.join(rootDir, "index.html");
if (fs.existsSync(indexPath)) {
  app.get("/", (_req, res) => {
    res.sendFile(indexPath);
  });
}

app.listen(PORT, () => {
  console.log(`Arcane compiler backend running on http://localhost:${PORT}`);
});
