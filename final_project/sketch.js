let rows = 20;
let cols = 20;
let gridSize, cellSize, marginX, marginY;
let players = [];
let currentPlayer = 0;
let diceRoll = 1;
let rolling = false;
let rollStartTime = 0;
let rollDuration = 1000; // 骰子動畫持續 1 秒
let path = [];

// 玩家移動動畫
let moving = false;
let moveSteps = 0;
let moveIndex = 0;
let stepDuration = 200; // 每格移動的時間，單位為毫秒

let showPosition = false; // 控制是否顯示位置
let rollButton; // 擲骰按鈕

function setup() {
  createCanvas(windowWidth, windowHeight);
  noFill();
  stroke(0);
  
  gridSize = min(width, height) * 0.8;
  cellSize = gridSize / max(rows, cols);
  
  // 偏移量，將格子網格往右下方偏移
  marginX = (width - gridSize) / 2 + 50;
  marginY = (height - gridSize) / 2 + 50;
  
  calculatePath();
  
  for (let i = 0; i < 4; i++) {
    players.push({ 
      index: 0, 
      color: color(random(100, 255), random(100, 255), random(100, 255)) 
    });
  }

  // 建立擲骰按鈕
  rollButton = createButton('擲骰子');
  rollButton.position(20, 60);
  rollButton.mousePressed(rollDice);
}

function draw() {
  background(255);
  
  // 繪製外圍格子
  for (let i = 0; i < rows; i++) {
    for (let j = 0; j < cols; j++) {
      let x = marginX + i * cellSize;
      let y = marginY + j * cellSize;

      if (i == 0 || i == rows - 1 || j == 0 || j == cols - 1) {
        fill(0, 0, 0, 255);
        rect(x, y, cellSize, cellSize);
      }
      
      // 高亮顯示當前玩家所在格子
      if (showPosition) {
        let currentPlayerPos = path[players[currentPlayer].index];
        if (abs(x - currentPlayerPos.x) < 1 && abs(y - currentPlayerPos.y) < 1) {
          fill(255, 0, 0, 150); // 紅色半透明
          rect(currentPlayerPos.x, currentPlayerPos.y, cellSize, cellSize);
        }
      }
    }
  }
  
  // 繪製玩家
  for (let i = 0; i < players.length; i++) {
    let { index, color } = players[i];
    let pos = path[index];

    fill(color);
    ellipse(pos.x, pos.y, cellSize * 0.6);
  }

  // 處理移動動畫
  if (moving) {
    let player = players[currentPlayer];
    
    if (millis() - player.moveStartTime >= stepDuration) {
      moveIndex = (moveIndex + 1) % path.length;
      player.index = moveIndex;
      player.moveStartTime = millis();

      moveSteps--;
      if (moveSteps <= 0) {
        moving = false;
        currentPlayer = (currentPlayer + 1) % players.length;
      }
    }
  }

  // 顯示當前玩家資訊
  fill(0);
  textSize(20);
  textAlign(LEFT, TOP);
  text(`現在輪到玩家 ${currentPlayer + 1}`, 20, 20);
  
  // 顯示正在移動的玩家小球
  let currentPlayerColor = players[currentPlayer].color;
  fill(currentPlayerColor);
  noStroke();
  ellipse(width / 2, 50, 30, 30);
  
  // 擲骰動畫
  if (rolling) {
    let elapsed = millis() - rollStartTime;
    if (elapsed < rollDuration) {
      diceRoll = floor(random(1, 7));
    } else {
      rolling = false;
    }
  }
  
  // 顯示骰子
  drawDice(width / 2, height - 150, diceRoll);
}

// 計算外圍移動路徑（順時針）
function calculatePath() {
  for (let i = 0; i < cols; i++) path.push({ x: marginX + i * cellSize, y: marginY });
  for (let i = 1; i < rows; i++) path.push({ x: marginX + (cols - 1) * cellSize, y: marginY + i * cellSize });
  for (let i = cols - 2; i >= 0; i--) path.push({ x: marginX + i * cellSize, y: marginY + (rows - 1) * cellSize });
  for (let i = rows - 2; i > 0; i--) path.push({ x: marginX, y: marginY + i * cellSize });
}

// 畫骰子
function drawDice(x, y, number) {
  fill(255);
  stroke(0);
  strokeWeight(4);
  rectMode(CENTER);
  rect(x, y, 80, 80, 10);
  
  fill(0);
  noStroke();
  textSize(32);
  textAlign(CENTER, CENTER);
  text(number, x, y);
}

// 按下擲骰按鈕觸發
function rollDice() {
  if (!rolling && !moving) {
    rolling = true;
    rollStartTime = millis();

    // 呼叫後端 GameServlet 的 "start" action
    fetch('/game?action=start')
      .then(response => response.json())
      .then(data => {
        console.log(data.message); // 顯示遊戲初始化消息
      })
      .catch(error => {
        console.error('Error:', error);
      });

    setTimeout(() => {
      let finalRoll = floor(random(1, 7));
      diceRoll = finalRoll;
      moveSteps = finalRoll;
      moveIndex = players[currentPlayer].index;
      moving = true;
      players[currentPlayer].moveStartTime = millis();
    }, rollDuration);
  }
}

// 顯示/隱藏位置功能仍保留
function keyPressed() {
  if (key === 'q' || key === 'Q') {
    showPosition = !showPosition;
  }
}