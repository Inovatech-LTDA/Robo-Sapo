import { app, BrowserWindow, ipcMain } from "electron";
import path from "node:path";
import started from "electron-squirrel-startup";
import { execFile } from "node:child_process";
import { promisify } from "node:util";

if (started) {
  app.quit();
}

const createWindow = () => {
  const mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, "preload.js"),
    },
  });

  if (MAIN_WINDOW_VITE_DEV_SERVER_URL) {
    mainWindow.loadURL(MAIN_WINDOW_VITE_DEV_SERVER_URL);
  } else {
    mainWindow.loadFile(
      path.join(__dirname, `../renderer/${MAIN_WINDOW_VITE_NAME}/index.html`),
    );
  }

  mainWindow.webContents.openDevTools();
};

app.on("ready", createWindow);

app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    app.quit();
  }
});

app.on("activate", () => {
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow();
  }
});

const execFileAsync = promisify(execFile);

let controller: AbortController | null = null;
ipcMain.handle("run-controller", async (_event, targetPath: string) => {
  try {
    const controller = new AbortController();
    const { signal } = controller;
    const { stdout } = await execFileAsync(targetPath, {
      signal,
    });
    return stdout;
  } catch (error: any) {
    const stderr = error.stderr ? error.stderr.toString() : "";
    if (error.name === "AbortError" || error.code === "ABORT_ERR" || error.signal === "SIGTERM") {
      return null
    }
    throw new Error(
      `Falha ao executar ${targetPath}: ${stderr || error.message}`,
    );
   
  }
  finally {
    controller = null
  }
});

ipcMain.handle("abortController", async () => {
  try {
    if (controller) {
      controller.abort();
      controller = null
    }
  } catch (e) {
    console.log(e);
  }
});
