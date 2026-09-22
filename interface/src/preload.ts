import { contextBridge, ipcRenderer } from "electron";

contextBridge.exposeInMainWorld("versions", {
  node: () => process.versions.node,
  chrome: () => process.versions.chrome,
  electron: () => process.versions.electron,
});

contextBridge.exposeInMainWorld("controller", {
  runController: (targetPath: string) => ipcRenderer.invoke("run-controller", targetPath),
});

contextBridge.exposeInMainWorld("abortController", {
  abort: () => ipcRenderer.invoke("abortController"),
});