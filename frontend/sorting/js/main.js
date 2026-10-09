/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */

//main.js will mainly be used for the backend-server communication (pinging for loading screen)
//and other network-related stuff

document.addEventListener('DOMContentLoaded', () => {
    const playButton = document.querySelector('.play-button'); 
    
    if (!playButton) return;

    playButton.addEventListener('click', async (e) => {
        e.preventDefault();

        const urlParams = new URLSearchParams(window.location.search);
        const algoKey = urlParams.get('algo') || 'selection';

        // read server setting from switch
        const serverRadio = document.querySelector('input[name="server-target"]:checked');
        const selectedServer = serverRadio ? serverRadio.value : 'remote';

        // incase lh choosen, redir instant
        if (selectedServer === 'local') {
            window.location.href = `visualizer.html?algo=${algoKey}&server=local`;
            return;
        }

        const backendPingUrl = 'https://algosee.onrender.com/status';
        playButton.innerText = "checking server...";

        try {
            const controller = new AbortController();
            const timeoutId = setTimeout(() => controller.abort(), 3000);

            const response = await fetch(backendPingUrl, { 
                method: 'GET',
                signal: controller.signal 
            });
            clearTimeout(timeoutId);

            if (response.ok) {
                window.location.href = `visualizer.html?algo=${algoKey}&server=remote`;
            } else {
                window.location.href = `../loading.html?dir=sorting&algo=${algoKey}&server=remote`;
            }
        } catch (error) {
            window.location.href = `../loading.html?dir=sorting&algo=${algoKey}&server=remote`;
        }
    });
});