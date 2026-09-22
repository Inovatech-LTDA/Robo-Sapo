export {};

declare global {
  interface Window {
    versions: {
      node: () => string;
      chrome: () => string;
      electron: () => string;
    };
    controller: {
      runController: (targetPath: string) => Promise<string>;
    };
    abortController: {
      abort: () => Promise<>;
    };
  }
}
