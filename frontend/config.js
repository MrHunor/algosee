/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */

const LOCAL_BACKEND = 'http://localhost:8080';
const REMOTE_BACKEND = 'https://algosee.onrender.com';

/*
 * This config's objective is routing the visualization
 * depending on server-parameters (?server=local/remote)
 * and the switch in algo.html 
 */
function getBackendBase() {
    const urlParams = new URLSearchParams(window.location.search);
    let serverParam = urlParams.get('server');

    //incase no URL-parameter, check the switch in algo.html
    if (!serverParam) {
        const serverRadio = document.querySelector('input[name="server-target"]:checked');
        if (serverRadio) {
            serverParam = serverRadio.value;
        }
    }

    return (serverParam === 'local') ? LOCAL_BACKEND : REMOTE_BACKEND;
}

/*
 * ping URL
 */
function getBackendStatusUrl() {
    return `${getBackendBase()}/status`;
}

/*
 * generates endpoint URL for said algorithm
 */
function getAlgoApiUrl(algoKey) {
    return `${getBackendBase()}/sortalgo?algo=${algoKey}`;
}
