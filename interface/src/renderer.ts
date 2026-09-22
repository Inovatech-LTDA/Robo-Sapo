import "./index.css";



const keyElements: Record<string, HTMLElement | null> = {
  KeyA: document.getElementById("keyA"),
  KeyW: document.getElementById("keyW"),
  KeyS: document.getElementById("keyS"),
  KeyD: document.getElementById("keyD"),
};

window.addEventListener("keydown", (event) => {
  const element = keyElements[event.code];
  if (element) {
    element.classList.add("active");
  }
});

window.addEventListener("keyup", (event) => {
  const element = keyElements[event.code];
  if (element) {
    element.classList.remove("active");
  }
});

// 2. Manipulação de Dados dos Sensores na UI
interface SensorData {
  distance: number; // em cm
  battery: number; // 0-100%
  speed: number; // km/h
  connected: boolean;
}

function updateDashboard(data: SensorData) {
  // Atualizar Distância
  const distText = document.getElementById("distText");
  const distBar = document.getElementById("distBar");
  const obstacleStatus = document.getElementById("obstacleStatus");

  if (distText && distBar) {
    distText.innerText = `${data.distance} cm`;
    // Mapeia até 100cm para 100% da barra
    const percentage = Math.min(Math.max(data.distance, 0), 100);
    distBar.style.width = `${percentage}%`;

    // Alerta de obstáculo próximo
    if (data.distance < 20 && obstacleStatus) {
      distBar.style.backgroundColor = "#ef4444";
      obstacleStatus.innerText = "PERIGO";
      obstacleStatus.className = "value status-alert";
    } else if (obstacleStatus) {
      distBar.style.backgroundColor = "#3b82f6";
      obstacleStatus.innerText = "Livre";
      obstacleStatus.className = "value status-ok";
    }
  }

  // Atualizar Bateria
  const batteryText = document.getElementById("batteryText");
  const batteryBar = document.getElementById("batteryBar");
  if (batteryText && batteryBar) {
    batteryText.innerText = `${data.battery}%`;
    batteryBar.style.width = `${data.battery}%`;
  }

  // Atualizar Velocidade
  const speedValue = document.getElementById("speedValue");
  if (speedValue) {
    speedValue.innerHTML = `${data.speed} <small>km/h</small>`;
  }

  // Status de Ligação
  const connectionStatus = document.getElementById("connectionStatus");
  const statusText = document.getElementById("statusText");
  if (connectionStatus && statusText) {
    if (data.connected) {
      connectionStatus.classList.add("connected");
      statusText.innerText = "Conectado";
    } else {
      connectionStatus.classList.remove("connected");
      statusText.innerText = "Desconectado";
    }
  }
}

const messages = document.getElementById("messages");

async function executecmd() {
  try {
    const targetPath =
      "/Users/lucaslaguilio/Projects/Robo-Sapo/src/dist/Controller/Controller";
    const output = await window.controller.runController(targetPath);
    console.log("Controller started:", output);
    messages.innerText = `Controloador iniciado`;
    if (output.includes('Erro')) {
      messages.innerText = `${output}`;
      window.abortController.abort()
    }
  } catch (err) {
    messages.innerText = `Error: ${err}}`;
    console.error("Failed to run controller:", err);
  }
}

setInterval(() => {
  const simulatedData: SensorData = {
    distance: Math.floor(Math.random() * 80) + 10,
    battery: 85,
    speed: Math.floor(Math.random() * 15),
    connected: true,
  };

  updateDashboard(simulatedData);
}, 1000);

// inicia apenas uma vez a cada 30 segundos e uma na inicialização, se der erro espera os 30 segundos e inicia de novo
// fiz isso para não ficar abrindo um monte de processo/arquivo quando o controlador dar erro.
executecmd() 
setInterval(() => {
  executecmd() 
}, 30000)
const toggleLed = document.getElementById("toggleLed") as HTMLInputElement;
toggleLed?.addEventListener("change", (e) => {
  const isChecked = (e.target as HTMLInputElement).checked;
  console.log(`LED ${isChecked ? "Ligado" : "Desligado"}`);
});
