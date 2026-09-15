document.addEventListener('DOMContentLoaded', () => {
    //searches for start-vis. button on algo.html
    const playButton = document.querySelector('.play-button'); 
    
    if (!playButton) return; // in case it's site w/o the button, cancel

    playButton.addEventListener('click', async (e) => {
        e.preventDefault(); // prevents instant loading of deafult link

        //which algorithm is selected? (e.g. ?algo=bogo)
        const urlParams = new URLSearchParams(window.location.search);
        const algoKey = urlParams.get('algo') === null ? 'selection' : urlParams.get('algo');

        // URL to be replaced
        const backendPingUrl = 'https://idktheurlyet.com';

        // optional: give user feedback
        playButton.innerText = "checking server...";

        try {
            // ping get's max 2 secs
            const controller = new AbortController();
            const timeoutId = setTimeout(() => controller.abort(), 2000);

            const response = await fetch(backendPingUrl, { 
                method: 'GET',
                signal: controller.signal 
            });
            clearTimeout(timeoutId);

            if (response.ok) {
                // server awake -> redir to visualizer
                window.location.href = `visualizer.html?algo=${algoKey}`;
                return;
            }
        } catch (error) {
            // server sleeping (timout/error) -> loading screen
            window.location.href = `loading.html?algo=${algoKey}`;
        }
    });
});