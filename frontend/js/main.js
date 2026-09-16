//main.js will mainly be used for the backend-server communication (pinging for loading screen)
//and other network-related stuff

document.addEventListener('DOMContentLoaded', () => {
    //searches for start-vis. button on algo.html
    const playButton = document.querySelector('.play-button'); 
    
    if (!playButton) return; // in case it's site w/o the button, cancel

    playButton.addEventListener('click', async (e) => {
        e.preventDefault(); // prevents instant loading of deafult link

        //which algorithm is selected? (e.g. ?algo=bogo)
        const urlParams = new URLSearchParams(window.location.search);
        const algoKey = urlParams.get('algo') === null ? 'selection' : urlParams.get('algo');

        // URL incl. /status endpoint
        const backendPingUrl = 'https://algosee.onrender.com/status';

        // optional: give user feedback
        playButton.innerText = "checking server...";

        try {
            // ping get's max 3 secs
            const controller = new AbortController();
            const timeoutId = setTimeout(() => controller.abort(), 3000);

            const response = await fetch(backendPingUrl, { 
                method: 'GET',
                signal: controller.signal 
            });
            clearTimeout(timeoutId);

            if (response.ok) {
                // server awake -> redir to visualizer
                window.location.href = `visualizer.html?algo=${algoKey}`;
                return;
            } else {
                // server antwortet nicht mit 200 OK -> loading screen
                window.location.href = `loading.html?algo=${algoKey}`;
            }
        } catch (error) {
            // server sleeping (timeout/error) -> loading screen
            window.location.href = `loading.html?algo=${algoKey}`;
        }
    });
});