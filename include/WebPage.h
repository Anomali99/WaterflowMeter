#include <Arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Monitoring Aliran Air</title>
    <style>
        :root {
            --bg-color: #0f172a;
            --card-bg: #1e293b;
            --border-color: #334155;
            --text-main: #f8fafc;
            --text-muted: #94a3b8;
            --primary: #38bdf8;
            --primary-hover: #0ea5e9;
            --success: #10b981;
            --danger: #ef4444;
            --danger-hover: #dc2626;
            --input-bg: #0f172a;
            --box-bg: rgba(15, 23, 42, 0.5);
            --box-border: rgba(51, 65, 85, 0.5);
            --msg-bg: rgba(30, 41, 59, 0.5);
            --bar-bg: rgba(30, 41, 59, 0.8);
            --btn-hover-bg: var(--border-color);
            --btn-hover-text: #fff;
        }

        /* Tema Terang (Bisa di-toggle manual) */
        :root[data-theme="light"] {
            --bg-color: #f8fafc;
            --card-bg: #ffffff;
            --border-color: #e2e8f0;
            --text-main: #0f172a;
            --text-muted: #64748b;
            --primary: #0284c7;
            --primary-hover: #0369a1;
            --success: #059669;
            --danger: #dc2626;
            --danger-hover: #b91c1c;
            --input-bg: #f1f5f9;
            --box-bg: rgba(241, 245, 249, 0.8);
            --box-border: rgba(203, 213, 225, 0.8);
            --msg-bg: rgba(241, 245, 249, 0.8);
            --bar-bg: rgba(255, 255, 255, 0.8);
            --btn-hover-bg: #cbd5e1;
            --btn-hover-text: #0f172a;
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
        }

        body {
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
            background-color: var(--bg-color);
            color: var(--text-main);
            display: flex;
            flex-direction: column;
            align-items: center;
            min-height: 100vh;
            padding: 20px;
        }

        .container {
            width: 100%;
            max-width: 650px;
            display: flex;
            flex-direction: column;
            gap: 24px;
        }

        header {
            text-align: center;
            margin-bottom: 10px;
        }
        header h1 {
            color: var(--primary);
            font-size: 28px;
            margin-bottom: 5px;
        }
        header p {
            color: var(--text-muted);
            font-size: 14px;
        }

        .grid-cards {
            display: grid;
            grid-template-columns: 1fr;
            gap: 20px;
        }
        @media (min-width: 500px) {
            .grid-cards { grid-template-columns: 1fr 1fr; }
        }

        .card {
            background-color: var(--card-bg);
            border: 1px solid var(--border-color);
            border-radius: 16px;
            padding: 24px;
            text-align: center;
            position: relative;
            box-shadow: 0 4px 6px -1px rgba(0, 0, 0, 0.1);
            transition: transform 0.2s;
        }
        .card:hover { transform: translateY(-2px); }
        
        .card h2 {
            color: var(--text-muted);
            font-size: 12px;
            text-transform: uppercase;
            letter-spacing: 1px;
            margin-bottom: 15px;
        }
        
        .value-display {
            display: flex;
            justify-content: center;
            align-items: baseline;
            gap: 8px;
        }
        .value {
            font-size: 48px;
            font-weight: 800;
        }
        .unit {
            color: var(--text-muted);
            font-size: 18px;
        }
        .text-flow { color: var(--primary); }
        .text-vol { color: var(--success); }

        .btn {
            border: none;
            padding: 10px 20px;
            border-radius: 8px;
            font-weight: bold;
            cursor: pointer;
            transition: background 0.2s;
            display: inline-flex;
            align-items: center;
            justify-content: center;
            gap: 8px;
            font-size: 14px;
        }
        .btn-primary { background-color: var(--primary); color: #fff; }
        .btn-primary:hover { background-color: var(--primary-hover); }
        .btn-danger { background-color: transparent; color: var(--danger); border: 1px solid var(--danger); padding: 5px 10px; font-size: 12px;}
        .btn-danger:hover { background-color: rgba(239, 68, 68, 0.1); }
        .btn-outline { background-color: transparent; color: var(--text-muted); border: 1px solid var(--border-color); }
        .btn-outline:hover { background-color: var(--btn-hover-bg); color: var(--btn-hover-text); }

        .btn-reset-small {
            position: absolute;
            top: 15px;
            right: 15px;
            background: rgba(239, 68, 68, 0.1);
            color: var(--danger);
            border: 1px solid rgba(239, 68, 68, 0.3);
            border-radius: 6px;
            padding: 4px 8px;
            font-size: 10px;
            cursor: pointer;
            text-transform: uppercase;
            font-weight: bold;
        }
        .btn-reset-small:hover { background: var(--danger); color: white; }

        .control-group {
            display: flex;
            flex-direction: column;
            gap: 15px;
        }
        @media (min-width: 500px) {
            .control-group { flex-direction: row; align-items: center; }
        }

        .select-wrapper {
            background-color: var(--input-bg);
            border: 1px solid var(--border-color);
            border-radius: 8px;
            display: flex;
            align-items: center;
            flex-grow: 1;
            padding: 0 10px;
        }
        .select-wrapper label {
            color: var(--text-muted);
            font-size: 14px;
            margin-right: 10px;
        }
        select {
            background: transparent;
            color: var(--text-main);
            border: none;
            padding: 12px 0;
            width: 100%;
            outline: none;
            font-weight: bold;
            font-size: 14px;
        }
        select option { background-color: var(--input-bg); }

        .recording-active {
            display: none;
            background-color: rgba(239, 68, 68, 0.05);
            border: 1px solid rgba(239, 68, 68, 0.3);
            border-radius: 12px;
            padding: 15px;
            justify-content: space-between;
            align-items: center;
            flex-direction: column;
            gap: 15px;
        }
        @media (min-width: 500px) {
            .recording-active { flex-direction: row; }
        }
        .recording-active.show { display: flex; }

        .rec-indicator {
            display: flex;
            align-items: center;
            gap: 15px;
        }
        .dot {
            width: 15px; height: 15px;
            background-color: var(--danger);
            border-radius: 50%;
            animation: blink 1s infinite;
        }
        @keyframes blink { 50% { opacity: 0.3; } }
        
        .timer-text { font-size: 24px; font-weight: bold; font-family: monospace; }
        .rec-label { color: var(--danger); font-size: 12px; font-weight: bold; text-transform: uppercase; }

        .history-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 15px;
            padding: 0 5px;
        }
        .history-title {
            color: var(--text-muted);
            font-size: 14px;
            text-transform: uppercase;
            font-weight: bold;
        }
        
        .history-list { display: flex; flex-direction: column; gap: 12px; }
        
        .history-item {
            background-color: var(--card-bg);
            border: 1px solid var(--border-color);
            border-radius: 12px;
            padding: 15px;
            position: relative;
        }
        .history-item-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 15px;
            flex-wrap: wrap;
            gap: 10px;
            padding-right: 25px;
        }
        .history-input {
            background: transparent;
            border: none;
            border-bottom: 1px solid transparent;
            color: var(--primary);
            font-size: 16px;
            font-weight: bold;
            outline: none;
            width: 100%;
            max-width: 200px;
        }
        .history-input:focus, .history-input:hover { border-bottom: 1px solid var(--primary); }
        
        .history-meta {
            font-size: 11px;
            background: var(--input-bg);
            padding: 4px 8px;
            border-radius: 4px;
            color: var(--text-muted);
        }

        .history-stats {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 10px;
        }
        .stat-box {
            background: var(--box-bg);
            border: 1px solid var(--box-border);
            padding: 10px;
            border-radius: 8px;
        }
        .stat-label { font-size: 10px; color: var(--text-muted); text-transform: uppercase; margin-bottom: 5px;}
        .stat-val { font-size: 18px; font-weight: bold; }

        .btn-delete-item {
            position: absolute;
            top: 15px;
            right: 15px;
            background: none;
            border: none;
            color: var(--text-muted);
            cursor: pointer;
        }
        .btn-delete-item:hover { color: var(--danger); }

        .empty-msg {
            text-align: center;
            color: var(--text-muted);
            font-style: italic;
            font-size: 14px;
            padding: 20px;
            background: var(--msg-bg);
            border-radius: 12px;
            border: 1px dashed var(--border-color);
        }

        /* Tombol Tema */
        .theme-toggle {
            position: absolute;
            top: 20px;
            right: 20px;
            background: var(--card-bg);
            border: 1px solid var(--border-color);
            color: var(--text-main);
            width: 40px;
            height: 40px;
            border-radius: 50%;
            display: flex;
            align-items: center;
            justify-content: center;
            cursor: pointer;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
            transition: all 0.3s ease;
            z-index: 100;
        }
        .theme-toggle:hover {
            transform: scale(1.1);
        }

        .status-bar {
            margin-top: 30px;
            background-color: var(--bar-bg);
            border: 1px solid var(--border-color);
            padding: 10px 20px;
            border-radius: 30px;
            display: flex;
            align-items: center;
            gap: 10px;
            font-size: 12px;
        }
        .status-dot {
            width: 10px; height: 10px; border-radius: 50%;
            background-color: var(--danger);
        }
        .status-dot.connected { background-color: var(--success); }
    </style>
</head>
<body>

    <!-- Tombol Ganti Tema -->
    <button class="theme-toggle" id="themeToggleBtn" title="Ganti Tema">
        <svg id="themeIcon" width="20" height="20" fill="none" stroke="currentColor" viewBox="0 0 24 24"></svg>
    </button>

    <div class="container">
        <header>
            <h1>Dashboard WaterflowMeter</h1>
        </header>

        <main class="grid-cards">
            <div class="card">
                <h2>Debit Air Saat Ini</h2>
                <div class="value-display">
                    <span class="value text-flow" id="flowRate">0.0</span>
                    <span class="unit">L/min</span>
                </div>
            </div>

            <div class="card">
                <h2>Total Volume</h2>
                <button class="btn-reset-small" onclick="resetTotalVolume()" title="Reset Total Volume ke 0">Reset</button>
                <div class="value-display">
                    <span class="value text-vol" id="totalVolume">0.00</span>
                    <span class="unit">Liter</span>
                </div>
            </div>
        </main>

        <div class="card" style="text-align: left;">
            <h2 style="border-bottom: 1px solid var(--border-color); padding-bottom: 10px; margin-bottom: 20px;">Perekaman Manual</h2>

            <div class="control-group" id="recordSetup">
                <div class="select-wrapper">
                    <label for="durationSelect">Durasi:</label>
                    <select id="durationSelect">
                        <option value="30">30 Detik</option>
                        <option value="60" selected>1 Menit</option>
                        <option value="300">5 Menit</option>
                    </select>
                </div>
                <button class="btn btn-primary" onclick="startRecording()" style="width: 100%;">
                    Mulai Rekam
                </button>
            </div>

            <div class="recording-active" id="recordingActive">
                <div class="rec-indicator">
                    <div class="dot"></div>
                    <div>
                        <div class="rec-label">Sedang Merekam</div>
                        <div class="timer-text" id="countdownTimer">01:00</div>
                    </div>
                </div>
                <button class="btn btn-outline" onclick="cancelRecording()" style="width: 100%; max-width: 120px;">Batalkan</button>
            </div>
        </div>

        <div>
            <div class="history-header">
                <span class="history-title">Riwayat Rekaman</span>
                <button class="btn btn-danger" onclick="clearAllHistory()" id="btnClearHistory" style="display: none;">Hapus Semua</button>
            </div>
            
            <div class="history-list" id="historyContainer">
                <div class="empty-msg" id="emptyHistoryMsg">Belum ada data rekaman tersimpan.</div>
            </div>
        </div>
    </div>

    <div class="status-bar">
        <div class="status-dot" id="ws-indicator"></div>
        <span id="ws-status" style="color: var(--text-muted);">Menghubungkan...</span>
    </div>

    <script>
        var gateway = `ws://${window.location.hostname}/ws`;
        var websocket;
        
        var currentFlow = 0.0;
        var currentVolume = 0.0;

        var isRecording = false;
        var recordDurationSec = 60;
        var recordRemainingSec = 0;
        var startVol = 0.0;
        var recordTimer = null;
        var recordHistory = [];

        // --- LOGIKA TEMA ---
        const themeToggleBtn = document.getElementById('themeToggleBtn');
        const themeIcon = document.getElementById('themeIcon');
        const sunIcon = '<path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M12 3v1m0 16v1m9-9h-1M4 12H3m15.364 6.364l-.707-.707M6.343 6.343l-.707-.707m12.728 0l-.707.707M6.343 17.657l-.707.707M16 12a4 4 0 11-8 0 4 4 0 018 0z"></path>';
        const moonIcon = '<path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M20.354 15.354A9 9 0 018.646 3.646 9.003 9.003 0 0012 21a9.003 9.003 0 008.354-5.646z"></path>';

        let currentTheme = localStorage.getItem('theme');
        if (!currentTheme) {
            // Jika belum ada memori, ikuti pengaturan sistem/browser
            currentTheme = window.matchMedia('(prefers-color-scheme: light)').matches ? 'light' : 'dark';
        }
        
        // Terapkan tema ke elemen HTML utama
        document.documentElement.setAttribute('data-theme', currentTheme);
        themeIcon.innerHTML = currentTheme === 'light' ? moonIcon : sunIcon;

        // Aksi klik tombol tema
        themeToggleBtn.addEventListener('click', () => {
            currentTheme = document.documentElement.getAttribute('data-theme') === 'light' ? 'dark' : 'light';
            document.documentElement.setAttribute('data-theme', currentTheme);
            localStorage.setItem('theme', currentTheme); // Simpan di memori peramban
            themeIcon.innerHTML = currentTheme === 'light' ? moonIcon : sunIcon;
        });

        window.addEventListener('load', initWebSocket);

        function initWebSocket() {
            if(window.location.hostname === '' || window.location.hostname === 'localhost' || window.location.protocol === 'file:') {
                document.getElementById('ws-status').innerText = 'Mode Preview (Data Simulasi)';
                document.getElementById('ws-indicator').style.backgroundColor = 'orange';
                simulateData();
                return;
            }

            websocket = new WebSocket(gateway);
            websocket.onopen    = onOpen;
            websocket.onclose   = onClose;
            websocket.onmessage = onMessage;
        }

        function onOpen(event) {
            document.getElementById('ws-indicator').classList.add('connected');
            document.getElementById('ws-status').innerText = 'Terhubung ke ESP32';
        }

        function onClose(event) {
            document.getElementById('ws-indicator').classList.remove('connected');
            document.getElementById('ws-status').innerText = 'Terputus. Menghubungkan ulang...';
            setTimeout(initWebSocket, 2000);
        }

        function onMessage(event) {
            var myObj = JSON.parse(event.data);
            updateDashboard(myObj.flowRate, myObj.totalVolume);
        }

        function updateDashboard(flow, volume) {
            currentFlow = flow;
            currentVolume = volume;
            document.getElementById('flowRate').innerHTML = currentFlow.toFixed(1);
            document.getElementById('totalVolume').innerHTML = currentVolume.toFixed(2);
        }

        function resetTotalVolume() {
            if(confirm("Apakah Anda yakin ingin mereset Total Volume kembali ke 0?")) {
                if(websocket && websocket.readyState === WebSocket.OPEN) {
                    websocket.send(JSON.stringify({ command: "reset_volume" }));
                }
                
                currentVolume = 0;
                if(isRecording) startVol = 0; 
                document.getElementById('totalVolume').innerHTML = "0.00";
            }
        }

        function clearAllHistory() {
            if(confirm("Hapus seluruh riwayat rekaman secara permanen?")) {
                recordHistory = [];
                renderHistory();
            }
        }

        function startRecording() {
            if(isRecording) return;
            
            recordDurationSec = parseInt(document.getElementById('durationSelect').value);
            recordRemainingSec = recordDurationSec;
            startVol = currentVolume; 
            isRecording = true;

            document.getElementById('recordSetup').style.display = 'none';
            document.getElementById('recordingActive').classList.add('show');
            updateCountdownUI();

            recordTimer = setInterval(() => {
                recordRemainingSec--;
                updateCountdownUI();
                
                if(recordRemainingSec <= 0) finishRecording();
            }, 1000);
        }

        function updateCountdownUI() {
            let m = Math.floor(recordRemainingSec / 60).toString().padStart(2, '0');
            let s = (recordRemainingSec % 60).toString().padStart(2, '0');
            document.getElementById('countdownTimer').innerText = `${m}:${s}`;
        }

        function cancelRecording() {
            clearInterval(recordTimer);
            isRecording = false;
            document.getElementById('recordingActive').classList.remove('show');
            document.getElementById('recordSetup').style.display = 'flex';
        }

        function finishRecording() {
            cancelRecording();
            
            let recordedVol = currentVolume - startVol; 
            if (recordedVol < 0) recordedVol = 0; 
            
            let timeInMinutes = recordDurationSec / 60.0;
            let avgFlow = recordedVol / timeInMinutes;

            let now = new Date();
            let dateStr = now.toLocaleDateString('id-ID', { day: 'numeric', month: 'short', year: 'numeric' });
            let timeStr = now.toLocaleTimeString('id-ID', { hour: '2-digit', minute:'2-digit' });

            let newRecord = {
                id: Date.now(),
                title: "Rekaman " + (recordHistory.length + 1),
                duration: recordDurationSec,
                volume: recordedVol,
                avgFlow: avgFlow,
                date: `${dateStr} • ${timeStr}`
            };

            recordHistory.unshift(newRecord);
            renderHistory();
        }

        function renderHistory() {
            const container = document.getElementById('historyContainer');
            const btnClear = document.getElementById('btnClearHistory');

            if (recordHistory.length === 0) {
                container.innerHTML = '<div class="empty-msg" id="emptyHistoryMsg">Belum ada data rekaman tersimpan.</div>';
                btnClear.style.display = 'none';
                return;
            }

            btnClear.style.display = 'inline-block';

            container.innerHTML = recordHistory.map(rec => `
                <div class="history-item">
                    <div class="history-item-header">
                        <input type="text" class="history-input" value="${rec.title}" onchange="renameRecord(${rec.id}, this.value)">
                        <span class="history-meta">${rec.duration} Detik | ${rec.date}</span>
                    </div>
                    
                    <div class="history-stats">
                        <div class="stat-box">
                            <div class="stat-label">Volume Tercatat</div>
                            <div class="stat-val text-vol">${rec.volume.toFixed(2)} <span style="font-size:12px; font-weight:normal; color:var(--text-muted)">L</span></div>
                        </div>
                        <div class="stat-box">
                            <div class="stat-label">Rata-rata Debit</div>
                            <div class="stat-val text-flow">${rec.avgFlow.toFixed(1)} <span style="font-size:12px; font-weight:normal; color:var(--text-muted)">L/min</span></div>
                        </div>
                    </div>

                    <button class="btn-delete-item" onclick="deleteRecord(${rec.id})" title="Hapus rekaman ini">
                        <!-- Icon Trash SVG -->
                        <svg width="18" height="18" fill="none" stroke="currentColor" viewBox="0 0 24 24"><path stroke-linecap="round" stroke-linejoin="round" stroke-width="2" d="M19 7l-.867 12.142A2 2 0 0116.138 21H7.862a2 2 0 01-1.995-1.858L5 7m5 4v6m4-6v6m1-10V4a1 1 0 00-1-1h-4a1 1 0 00-1 1v3M4 7h16"></path></svg>
                    </button>
                </div>
            `).join('');
        }

        function renameRecord(id, newTitle) {
            let index = recordHistory.findIndex(r => r.id === id);
            if(index !== -1) recordHistory[index].title = newTitle;
        }

        function deleteRecord(id) {
            if(confirm("Hapus rekaman ini?")) {
                recordHistory = recordHistory.filter(r => r.id !== id);
                renderHistory();
            }
        }

        function simulateData() {
            let flow = 0.0;
            setInterval(() => {
                flow = Math.random() * 5 + 10; 
                currentVolume += (flow / 60); 
                updateDashboard(flow, currentVolume);
            }, 1000);
        }
    </script>
</body>
</html>
)rawliteral";