// filepath: /Users/xiaocui/Arduino/esp32-pico-D4/web-server/html.h
const char* html_page = R"=====(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 WiFi 配置</title>
    <style>
        body {
            font-family: 'Helvetica Neue', Arial, sans-serif;
            margin: 0;
            padding: 0;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .container {
            width: 90%;
            max-width: 450px;
            margin: 20px auto;
            background-color: rgba(255, 255, 255, 0.95);
            padding: 25px;
            border-radius: 12px;
            box-shadow: 0 8px 20px rgba(0,0,0,0.2);
        }
        h1 {
            text-align: center;
            color: #333;
            font-size: 28px;
            margin-bottom: 25px;
            font-weight: 600;
            background: linear-gradient(to right, #667eea, #764ba2);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
        }
        form {
            display: flex;
            flex-direction: column;
        }
        label {
            margin-top: 12px;
            margin-bottom: 6px;
            color: #555;
            font-weight: 500;
            font-size: 15px;
        }
        input {
            padding: 12px 15px;
            margin-bottom: 16px;
            border: 1px solid #e0e0e0;
            border-radius: 8px;
            box-shadow: inset 0 1px 3px rgba(0,0,0,0.05);
            font-size: 16px;
            transition: all 0.3s ease;
        }
        input:focus {
            border-color: #667eea;
            box-shadow: 0 0 0 2px rgba(102, 126, 234, 0.2);
            outline: none;
        }
        button {
            background: linear-gradient(to right, #667eea, #764ba2);
            color: white;
            border: none;
            padding: 12px;
            border-radius: 8px;
            cursor: pointer;
            font-size: 16px;
            margin-bottom: 12px;
            font-weight: 500;
            transition: all 0.3s ease;
            box-shadow: 0 4px 6px rgba(0,0,0,0.1);
        }
        button:hover {
            transform: translateY(-2px);
            box-shadow: 0 7px 14px rgba(0,0,0,0.15);
        }
        .status {
            text-align: center;
            margin-top: 20px;
            color: #555;
            background-color: #f8f9fa;
            padding: 12px;
            border-radius: 8px;
            font-size: 14px;
        }
        .networks {
            max-height: 220px;
            overflow-y: auto;
            margin-bottom: 20px;
            border: 1px solid #e0e0e0;
            border-radius: 8px;
            padding: 8px;
            background-color: #f8f9fa;
            box-shadow: inset 0 2px 4px rgba(0,0,0,0.05);
        }
        .network-item {
            padding: 12px 15px;
            border-bottom: 1px solid #e0e0e0;
            cursor: pointer;
            display: flex;
            align-items: center;
            justify-content: space-between;
            transition: all 0.2s ease;
        }
        .network-item:hover {
            background-color: #eef0fd;
            transform: translateX(3px);
        }
        .network-item:last-child {
            border-bottom: none;
        }
        .refresh-btn {
            background: linear-gradient(to right, #4facfe, #00f2fe);
            margin-bottom: 15px;
            display: flex;
            align-items: center;
            justify-content: center;
        }
        .refresh-btn:hover {
            background: linear-gradient(to right, #38a0fc, #00ddf0);
        }
        .reset-btn {
            background: linear-gradient(to right, #ff758c, #ff7eb3);
            margin-top: 15px;
            display: flex;
            align-items: center;
            justify-content: center;
        }
        .reset-btn:hover {
            background: linear-gradient(to right, #ff6a7f, #ff70a6);
        }
        .save-btn {
            background: linear-gradient(to right, #6ab04c, #badc58);
        }
        .save-btn:hover {
            background: linear-gradient(to right, #5aa03c, #a9cb47);
        }
        .signal {
            font-size: 13px;
            background: rgba(102, 126, 234, 0.12);
            padding: 3px 8px;
            border-radius: 12px;
            color: #5a67d8;
            font-weight: 500;
            display: flex;
            align-items: center;
            gap: 5px;
        }
        .wifi-icon {
            margin-right: 10px;
            color: #667eea;
        }
        .loading {
            text-align: center;
            padding: 15px;
            color: #667eea;
        }
        .status-indicator {
            display: inline-block;
            width: 10px;
            height: 10px;
            border-radius: 50%;
            margin-right: 8px;
        }
        .waiting {
            background-color: #f6e58d;
        }
        .connected {
            background-color: #badc58;
        }
        .failed {
            background-color: #ff7979;
        }
        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
        .spinner {
            display: inline-block;
            width: 16px;
            height: 16px;
            border: 2px solid rgba(102, 126, 234, 0.3);
            border-radius: 50%;
            border-top-color: #667eea;
            animation: spin 1s linear infinite;
            margin-right: 8px;
        }
        .icon {
            width: 18px;
            height: 18px;
            margin-right: 8px;
            fill: currentColor;
            display: inline-block;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>ESP32 WiFi 配置</h1>
        <button class="refresh-btn" onclick="scanWiFi()">
            <svg class="icon" viewBox="0 0 24 24">
                <path d="M17.65 6.35C16.2 4.9 14.21 4 12 4c-4.42 0-7.99 3.58-7.99 8s3.57 8 7.99 8c3.73 0 6.84-2.55 7.73-6h-2.08c-.82 2.33-3.04 4-5.65 4-3.31 0-6-2.69-6-6s2.69-6 6-6c1.66 0 3.14.69 4.22 1.78L13 11h7V4l-2.35 2.35z"/>
            </svg>
            扫描WiFi
        </button>
        <div class="networks" id="wifi-list">
            <p style="text-align:center;">点击"扫描WiFi"按钮获取可用网络</p>
        </div>
        <form action="/save" method="post">
            <label for="ssid">WiFi 名称:</label>
            <input type="text" id="ssid" name="ssid" required placeholder="选择网络或手动输入">
            
            <label for="password">WiFi 密码:</label>
            <input type="password" id="password" name="password" placeholder="输入WiFi密码">
            
            <button class="save-btn" type="submit">
                <svg class="icon" viewBox="0 0 24 24">
                    <path d="M9 16.17L4.83 12l-1.42 1.41L9 19 21 7l-1.41-1.41z"/>
                </svg>
                保存配置
            </button>
        </form>
        <div class="status">
            <p><span class="status-indicator waiting"></span>设备状态: <span id="status">等待配置</span></p>
        </div>
        <form action="/reset" method="post">
            <button class="reset-btn" type="submit">
                <svg class="icon" viewBox="0 0 24 24">
                    <path d="M12 6v6l5 5-1.41 1.42L11 13.83V6h1zm0-5C6.48 1 2 5.48 2 11s4.48 10 10 10 10-4.48 10-10S17.52 1 12 1zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8z"/>
                </svg>
                重置配置
            </button>
        </form>
    </div>

    <script>
        function scanWiFi() {
            document.getElementById("wifi-list").innerHTML = "<p style='text-align:center;'><div class='spinner'></div>正在扫描...</p>";
            fetch('/scan')
                .then(response => response.json())
                .then(data => {
                    let html = "";
                    data.forEach(network => {
                        let signalIcon = "";
                        let signalStrength = "";
                        
                        if (network.rssi > -50) {
                            signalStrength = "强";
                            signalIcon = "<svg class='icon' viewBox='0 0 24 24'><path d='M12 3C7.41 3 3.86 4.53 0.89 6.59L12 23l11.11-16.41C20.14 4.53 16.59 3 12 3z'/></svg>";
                        } else if (network.rssi > -70) {
                            signalStrength = "中";
                            signalIcon = "<svg class='icon' viewBox='0 0 24 24'><path d='M12 3C7.41 3 3.86 4.53 0.89 6.59L12 23l11.11-16.41C20.14 4.53 16.59 3 12 3z' fill-opacity='0.7'/></svg>";
                        } else {
                            signalStrength = "弱";
                            signalIcon = "<svg class='icon' viewBox='0 0 24 24'><path d='M12 3C7.41 3 3.86 4.53 0.89 6.59L12 23l11.11-16.41C20.14 4.53 16.59 3 12 3z' fill-opacity='0.4'/></svg>";
                        }
                        
                        html += `<div class="network-item" onclick="selectNetwork('${network.ssid}')">
                            <div><svg class="icon" viewBox="0 0 24 24">
                                <path d="M1 9l2 2c4.97-4.97 13.03-4.97 18 0l2-2C16.93 2.93 7.08 2.93 1 9zm8 8l3 3 3-3c-1.65-1.66-4.34-1.66-6 0zm-4-4l2 2c2.76-2.76 7.24-2.76 10 0l2-2C15.14 9.14 8.87 9.14 5 13z"/>
                            </svg>${network.ssid}</div>
                            <span class="signal">${signalIcon}${signalStrength}</span>
                        </div>`;
                    });
                    document.getElementById("wifi-list").innerHTML = html || "<p style='text-align:center;'><svg class='icon' viewBox='0 0 24 24'><path d='M20 9c-.04-4.39-3.6-7.93-8-7.93S4.04 4.61 4 9v6c0 1.66 1.34 3 3 3h10c1.66 0 3-1.34 3-3V9zm-7 10c-1.1 0-2-.9-2-2h4c0 1.1-.9 2-2 2zm7-3H4v-2h16v2zm0-3H4V9c0-3.87 3.13-7 7-7s7 3.13 7 7v4z'/></svg>未找到WiFi网络</p>";
                    
                    // 添加网络项动画效果
                    const networkItems = document.querySelectorAll('.network-item');
                    networkItems.forEach((item, index) => {
                        item.style.opacity = '0';
                        item.style.transform = 'translateY(20px)';
                        setTimeout(() => {
                            item.style.transition = 'opacity 0.3s ease, transform 0.3s ease';
                            item.style.opacity = '1';
                            item.style.transform = 'translateY(0)';
                        }, index * 50);
                    });
                })
                .catch(error => {
                    document.getElementById("wifi-list").innerHTML = "<p style='text-align:center;'><svg class='icon' viewBox='0 0 24 24'><path d='M12 2C6.48 2 2 6.48 2 12s4.48 10 10 10 10-4.48 10-10S17.52 2 12 2zm1 15h-2v-2h2v2zm0-4h-2V7h2v6z'/></svg>扫描失败，请重试</p>";
                });
        }
        
        function selectNetwork(ssid) {
            document.getElementById("ssid").value = ssid;
            document.getElementById("password").focus();
            
            // 高亮显示选中的网络
            const networkItems = document.querySelectorAll('.network-item');
            networkItems.forEach(item => {
                if (item.textContent.includes(ssid)) {
                    item.style.backgroundColor = '#eef0fd';
                    item.style.borderLeft = '3px solid #667eea';
                } else {
                    item.style.backgroundColor = '';
                    item.style.borderLeft = '';
                }
            });
        }
        
        // 更新状态显示
        function updateStatus(status) {
            const statusElem = document.getElementById("status");
            const indicator = document.querySelector(".status-indicator");
            
            statusElem.textContent = status;
            
            if (status === "等待配置") {
                indicator.className = "status-indicator waiting";
            } else if (status.includes("成功")) {
                indicator.className = "status-indicator connected";
            } else if (status.includes("失败")) {
                indicator.className = "status-indicator failed";
            }
        }
        
        // 页面加载后自动扫描WiFi
        window.onload = function() {
            setTimeout(scanWiFi, 1000);
            
            // 表单提交前的验证
            document.querySelector('form').addEventListener('submit', function(event) {
                const ssid = document.getElementById('ssid').value.trim();
                const password = document.getElementById('password').value;
                
                if (!ssid) {
                    event.preventDefault();
                    alert('请选择或输入WiFi名称');
                    return false;
                }
                
                updateStatus("正在保存配置...");
            });
        };
    </script>
</body>
</html>
)=====";

const char* success_html = R"=====(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>配置成功</title>
    <style>
        body {
            font-family: 'Helvetica Neue', Arial, sans-serif;
            margin: 0;
            padding: 0;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .container {
            width: 90%;
            max-width: 450px;
            margin: 20px auto;
            background-color: rgba(255, 255, 255, 0.95);
            padding: 25px;
            border-radius: 12px;
            box-shadow: 0 8px 20px rgba(0,0,0,0.2);
            text-align: center;
        }
        h1 {
            color: #43a047;
            font-size: 28px;
            margin-bottom: 25px;
            font-weight: 600;
            position: relative;
            display: inline-block;
        }
        h1::after {
            content: '';
            position: absolute;
            bottom: -8px;
            left: 50%;
            transform: translateX(-50%);
            width: 60px;
            height: 4px;
            background: linear-gradient(to right, #43a047, #7cb342);
            border-radius: 2px;
        }
        p {
            color: #555;
            margin-bottom: 20px;
            font-size: 16px;
            line-height: 1.5;
        }
        .success-icon {
            width: 80px;
            height: 80px;
            margin: 20px auto;
            border-radius: 50%;
            background-color: #e8f5e9;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .checkmark {
            width: 40px;
            height: 40px;
            fill: #43a047;
        }
        .progress {
            width: 100%;
            height: 6px;
            background-color: #e0e0e0;
            border-radius: 3px;
            margin-top: 30px;
            position: relative;
            overflow: hidden;
        }
        .progress-bar {
            position: absolute;
            top: 0;
            left: 0;
            height: 100%;
            width: 0%;
            background: linear-gradient(to right, #43a047, #7cb342);
            animation: progress 3s linear forwards;
        }
        @keyframes progress {
            0% { width: 0%; }
            100% { width: 100%; }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="success-icon">
            <svg class="checkmark" viewBox="0 0 24 24">
                <path d="M9 16.17L4.83 12l-1.42 1.41L9 19 21 7l-1.41-1.41z"/>
            </svg>
        </div>
        <h1>配置成功!</h1>
        <p>WiFi 配置已保存。设备将尝试连接到指定的网络。</p>
        <p>如果连接成功，设备将退出配网模式。</p>
        <div class="progress">
            <div class="progress-bar"></div>
        </div>
    </div>
</body>
</html>
)=====";

const char* reset_html = R"=====(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>重置成功</title>
    <style>
        body {
            font-family: 'Helvetica Neue', Arial, sans-serif;
            margin: 0;
            padding: 0;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .container {
            width: 90%;
            max-width: 450px;
            margin: 20px auto;
            background-color: rgba(255, 255, 255, 0.95);
            padding: 25px;
            border-radius: 12px;
            box-shadow: 0 8px 20px rgba(0,0,0,0.2);
            text-align: center;
        }
        h1 {
            color: #e53935;
            font-size: 28px;
            margin-bottom: 25px;
            font-weight: 600;
            position: relative;
            display: inline-block;
        }
        h1::after {
            content: '';
            position: absolute;
            bottom: -8px;
            left: 50%;
            transform: translateX(-50%);
            width: 60px;
            height: 4px;
            background: linear-gradient(to right, #e53935, #ff8a80);
            border-radius: 2px;
        }
        p {
            color: #555;
            margin-bottom: 20px;
            font-size: 16px;
            line-height: 1.5;
        }
        .reset-icon {
            width: 80px;
            height: 80px;
            margin: 20px auto;
            border-radius: 50%;
            background-color: #ffebee;
            display: flex;
            justify-content: center;
            align-items: center;
        }
        .reset-symbol {
            width: 40px;
            height: 40px;
            fill: #e53935;
        }
        .progress {
            width: 100%;
            height: 6px;
            background-color: #e0e0e0;
            border-radius: 3px;
            margin-top: 30px;
            position: relative;
            overflow: hidden;
        }
        .progress-bar {
            position: absolute;
            top: 0;
            left: 0;
            height: 100%;
            width: 0%;
            background: linear-gradient(to right, #e53935, #ff8a80);
            animation: progress 3s linear forwards;
        }
        @keyframes progress {
            0% { width: 0%; }
            100% { width: 100%; }
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="reset-icon">
            <svg class="reset-symbol" viewBox="0 0 24 24">
                <path d="M12 5V2L8 6l4 4V7c3.31 0 6 2.69 6 6s-2.69 6-6 6-6-2.69-6-6H4c0 4.42 3.58 8 8 8s8-3.58 8-8-3.58-8-8-8z"/>
            </svg>
        </div>
        <h1>重置成功!</h1>
        <p>WiFi 配置已重置。设备将重新启动并进入配网模式。</p>
        <p>请等待几秒钟，然后重新连接到设备的AP热点。</p>
        <div class="progress">
            <div class="progress-bar"></div>
        </div>
    </div>
</body>
</html>
)=====";
