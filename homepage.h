const char* homepage_html = R"=====(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>设备主页</title>

<style>
    body {
        margin: 0;
        padding: 0;
        font-family: "Helvetica Neue", Arial, sans-serif;
        background: #111;
        color: #eee;
        display: flex;
        justify-content: center;
        align-items: center;
        min-height: 100vh;
        background-image: url('https://circus-of-science.de/strangerthings/img/st-bg.jpg');
        background-size: cover;
        background-position: center;
    }

    .container {
        width: 92%;
        max-width: 480px;
        background: rgba(0,0,0,0.65);
        border-radius: 16px;
        padding: 32px 26px;
        box-shadow: 0 0 32px rgba(0,0,0,0.65);
        text-align: center;
        backdrop-filter: blur(6px);
    }

    h1 {
        font-size: 38px;
        margin-bottom: 10px;
        font-weight: 800;
        color: #ff2a2a;
        text-shadow: 0 0 8px rgba(255, 0, 0, 0.55);
    }

    .subtitle {
        font-size: 18px;
        margin-bottom: 18px;
        color: #fff;
        opacity: 0.9;
        line-height: 1.5em;
    }

    .desc,
    .desc2 {
        font-size: 16px;
        color: #ddd;
        margin-bottom: 14px;
        line-height: 1.6;
    }

    .warn {
        font-size: 20px;
        font-weight: 700;
        margin-top: 18px;
        color: #ff5353;
        text-shadow: 0 0 6px rgba(255, 30, 30, 0.8);
    }

    .warn-detail {
        margin-top: 6px;
        font-size: 14px;
        line-height: 1.6;
        color: #ffdddd;
    }

    .input-box {
        margin-top: 22px;
        display: flex;
        gap: 10px;
        justify-content: center;
    }

    .msg-input {
        flex: 1;
        padding: 12px;
        font-size: 18px;
        border-radius: 8px;
        border: 2px solid #333;
        background: rgba(255,255,255,0.85);
        outline: none;
        text-transform: uppercase;
    }

    .msg-input:focus {
        border-color: #ff2a2a;
    }

    .send-btn {
        padding: 12px 22px;
        font-size: 18px;
        background: #ff2a2a;
        color: #fff;
        font-weight: bold;
        border: none;
        border-radius: 8px;
        cursor: pointer;
        transition: 0.2s;
    }

    .send-btn:hover {
        background: #ff4646;
    }
</style>

</head>
<body>

<div class="container">
    <h1>你好！</h1>

    <div class="subtitle">你可以在这里向我的<br>“怪奇物语”灯串发送消息！</div>

    <div class="desc">
        输入一条<strong>最多 25 个字符</strong>的消息，只能使用 A-Z 字母和空格。
        不支持中文、数字和符号。
    </div>

    <div class="desc2">每天最多允许发送 <strong>2 条消息</strong>。</div>

    <div class="warn">⚠️ 注意</div>
    <div class="warn-detail">
        为避免发送不友善或违法内容，页面将在你发送消息时记录你的 IP，
        保存 7 天，如有必要将提交至相关部门。大家请文明留言 🙏
    </div>

    <form class="input-box" id="msgForm" autocomplete="off">
        <input class="msg-input" id="msgInput" type="text" maxlength="25" placeholder="请输入你的消息" required pattern="[A-Za-z ]+">
        <button class="send-btn" type="submit">发送</button>
    </form>
    <div id="queueInfo" style="margin-top:18px;color:#ff2a2a;font-weight:bold;"></div>
    <script>
    function updateQueueInfo() {
      fetch('/queue').then(r=>r.json()).then(j=>{
        document.getElementById('queueInfo').innerText = '等待执行的消息数量：' + j.waiting;
      });
    }
    setInterval(updateQueueInfo, 1000);
    updateQueueInfo();
    document.getElementById('msgForm').onsubmit = function(e) {
        e.preventDefault();
        var msg = document.getElementById('msgInput').value.trim();
        if (!msg.match(/^[A-Za-z ]{1,25}$/)) {
            alert('只能输入字母和空格，且长度不超过25');
            return false;
        }
        fetch('/message', {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
            body: 'msg=' + encodeURIComponent(msg)
        }).then(r => r.text()).then(t => {alert(t);updateQueueInfo();});
        return false;
    };
    </script>

</div>

</body>
</html>
)=====";