const char* homepage_html = R"=====(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Stranger Things Light Wall</title>
    <!-- 引入类似剧集标题的字体 -->
    <link href="https://fonts.googleapis.com/css2?family=Benguiat+ITC&family=Permanent+Marker&display=swap" rel="stylesheet">
    <style>
        :root {
            --wall-color: #dcbfa3; /* 米黄色旧墙纸 */
            --wire-color: #1a1a1a;
            --bulb-off-opacity: 0.3;
        }

        body {
            margin: 0;
            padding: 0;
            background-color: #111;
            font-family: 'Permanent Marker', cursive; /* 手写字体 */
            display: flex;
            flex-direction: column;
            align-items: center;
            min-height: 100vh;
            overflow-x: hidden;
        }

        /* 墙面背景 */
        .wall-container {
            position: relative;
            width: 100%;
            max-width: 1000px;
            height: 600px;
            background: 
                radial-gradient(circle at center, rgba(0,0,0,0) 20%, rgba(0,0,0,0.6) 100%),
                url('https://www.transparenttextures.com/patterns/floral-linen.png'), /* 模拟花卉墙纸纹理 */
                linear-gradient(to bottom, #dcbfa3 0%, #c4a484 100%);
            background-blend-mode: multiply;
            display: flex;
            justify-content: center;
            align-items: center;
            flex-wrap: wrap;
            padding: 50px 20px;
            box-sizing: border-box;
            box-shadow: 0 0 50px rgba(0,0,0,0.8);
            border: 10px solid #2a2a2a;
            margin-top: 20px;
        }

        /* 字母区域布局 */
        .alphabet-grid {
            display: flex;
            flex-wrap: wrap;
            justify-content: center;
            width: 100%;
            position: relative;
        }

        .char-container {
            position: relative;
            width: 11%; /* 一行大约放8-9个 */
            height: 120px;
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: flex-end;
            margin-bottom: 20px;
        }

        /* 字母涂鸦 */
        .letter-paint {
            font-size: 3rem;
            color: #111;
            text-transform: uppercase;
            margin-top: 10px;
            opacity: 0.8;
            transform: rotate(-5deg); /* 稍微歪一点看起来像手绘 */
        }
        
        /* 稍微随机化一点字母的角度 */
        .char-container:nth-child(odd) .letter-paint { transform: rotate(3deg); }
        .char-container:nth-child(3n) .letter-paint { transform: rotate(-6deg); }

        /* 电线 (简单的贝塞尔曲线模拟) */
        .wire {
            position: absolute;
            top: 25px;
            left: -50%;
            width: 200%;
            height: 40px;
            border-bottom: 3px solid var(--wire-color);
            border-radius: 50%;
            z-index: 1;
            pointer-events: none;
        }

        /* 灯泡 */
        .bulb {
            width: 20px;
            height: 35px;
            border-radius: 50% 50% 40% 40%;
            background-color: #fff;
            opacity: var(--bulb-off-opacity);
            position: absolute;
            top: 30px;
            z-index: 2;
            transition: all 0.1s ease-in-out;
        }

        /* 灯泡底座 */
        .bulb::before {
            content: '';
            position: absolute;
            top: -5px;
            left: 5px;
            width: 10px;
            height: 8px;
            background: #222;
            border-radius: 2px;
        }

        /* 定义几种灯泡颜色 */
        .c-red { background-color: #ff3333; box-shadow: 0 0 5px #ff3333; }
        .c-blue { background-color: #3333ff; box-shadow: 0 0 5px #3333ff; }
        .c-green { background-color: #33ff33; box-shadow: 0 0 5px #33ff33; }
        .c-yellow { background-color: #ffff33; box-shadow: 0 0 5px #ffff33; }
        .c-pink { background-color: #ff33ff; box-shadow: 0 0 5px #ff33ff; }
        .c-white { background-color: #e0f7fa; box-shadow: 0 0 5px #e0f7fa; }

        /* 发光状态 */
        .bulb.active {
            opacity: 1;
            filter: brightness(1.5);
        }
        
        /* 强烈的辉光效果 */
        .c-red.active { box-shadow: 0 0 40px 10px #ff0000; }
        .c-blue.active { box-shadow: 0 0 40px 10px #0000ff; }
        .c-green.active { box-shadow: 0 0 40px 10px #00ff00; }
        .c-yellow.active { box-shadow: 0 0 40px 10px #ffff00; }
        .c-pink.active { box-shadow: 0 0 40px 10px #ff00ff; }
        .c-white.active { box-shadow: 0 0 40px 10px #ffffff; }

        /* 控制面板 */
        .controls {
            margin-top: 30px;
            display: flex;
            gap: 10px;
            z-index: 10;
        }

        input {
            padding: 10px 15px;
            font-size: 1.2rem;
            background: #333;
            border: 1px solid #555;
            color: #d12e2e; /* 红色字体呼应标题 */
            font-family: 'Courier New', monospace;
            border-radius: 5px;
            width: 300px;
        }

        input:focus {
            outline: none;
            border-color: #d12e2e;
        }

        button {
            padding: 10px 20px;
            font-size: 1.2rem;
            cursor: pointer;
            background: #d12e2e;
            color: white;
            border: none;
            border-radius: 5px;
            font-family: 'Permanent Marker', cursive;
            transition: transform 0.1s;
        }

        button:active {
            transform: scale(0.95);
        }
        
        button:disabled {
            background: #555;
            cursor: not-allowed;
        }

        /* 标题样式 */
        h1 {
            color: #d12e2e;
            text-shadow:
                0 0 20px #000,
                0 0 40px #ff2222,
                0 0 80px #ff2222,
                0 0 160px #ff2222,
                0 0 320px #ff2222;
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; /* 这里的字体需要特殊引入，暂时用普通字体带红色描边模拟 */
            /* 如果能加载到 Benguiat 字体效果最好 */
            font-family: serif; 
            text-transform: uppercase;
            font-size: 3rem;
            font-weight: 900;
            margin-top: 20px;
            -webkit-text-stroke: 1px #000;
            letter-spacing: 2px;
        }

        @media (max-width: 600px) {
            .char-container { width: 14%; height: 90px; }
            .letter-paint { font-size: 2rem; }
            .bulb { width: 15px; height: 25px; }
            input { width: 200px; }
        }
    </style>
</head>
<body>

    <!-- 标题 -->
    <h1>Stranger Things</h1>

    <!-- 墙面区域 -->
    <div class="wall-container">
        <div class="alphabet-grid" id="grid">
            <!-- 字母将通过JS生成 -->
        </div>
    </div>

    <!-- 控制区域 -->
    <div class="controls">
        <input type="text" id="messageInput" placeholder="RUN..." maxlength="50" autocomplete="off">
        <button onclick="transmitMessage()" id="sendBtn">Send Message</button>
    </div>

    <script>
        // 配置
        const alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const colors = ['c-white', 'c-blue', 'c-red', 'c-yellow', 'c-pink', 'c-green'];
        
        const grid = document.getElementById('grid');
        const input = document.getElementById('messageInput');
        const btn = document.getElementById('sendBtn');
        let isTransmitting = false;

        // 初始化墙面
        function initWall() {
            alphabet.split('').forEach((char, index) => {
                const container = document.createElement('div');
                container.className = 'char-container';
                container.id = `char-${char}`;

                // 分配随机但固定的颜色 (为了美观，按顺序循环分配)
                const colorClass = colors[index % colors.length];

                // 灯泡
                const bulb = document.createElement('div');
                bulb.className = `bulb ${colorClass}`;
                bulb.id = `bulb-${char}`;

                // 电线 (除了第一个，都有连线)
                if (index > 0) {
                    const wire = document.createElement('div');
                    wire.className = 'wire';
                    // 调整电线位置以连接上一个
                    wire.style.left = '-55%';
                    container.appendChild(wire);
                }

                // 字母涂鸦
                const letter = document.createElement('div');
                letter.className = 'letter-paint';
                letter.innerText = char;

                container.appendChild(bulb);
                container.appendChild(letter);
                grid.appendChild(container);
            });
        }

        // 辅助函数：延迟
        const delay = (ms) => new Promise(resolve => setTimeout(resolve, ms));

        // 核心逻辑：点亮字母
        async function lightUpLetter(char) {
            const bulb = document.getElementById(`bulb-${char}`);
            if (!bulb) return;

            // 点亮
            bulb.classList.add('active');
            
            // 保持亮起的时间
            await delay(800);

            // 熄灭
            bulb.classList.remove('active');
            
            // 字母间隔时间
            await delay(300);
        }

        // 发送消息
        async function transmitMessage() {
            if (isTransmitting) return;
            const text = input.value.toUpperCase().replace(/[^A-Z]/g, ''); // 只保留字母
            if (!text) return;

            // 发送到后端，后端会调用playMessage
            fetch('/message', {
                method: 'POST',
                headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
                body: 'msg=' + encodeURIComponent(text)
            });

            isTransmitting = true;
            btn.disabled = true;
            input.disabled = true;
            await delay(500);
            for (let char of text) {
                await lightUpLetter(char);
            }
            isTransmitting = false;
            btn.disabled = false;
            input.disabled = false;
            input.focus();
        }

        // 监听回车键
        input.addEventListener('keypress', function (e) {
            if (e.key === 'Enter') {
                transmitMessage();
            }
        });

        // 启动
        initWall();

    </script>
</body>
</html>
)=====";