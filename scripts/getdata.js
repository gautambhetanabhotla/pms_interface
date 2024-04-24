let numResults = 10, numFields = 1;
let APIKEY = null;
let channelID = null;
let fieldNames = ['spo2', 'hr', 'sysbp', 'diabp', 'waterlevel'];
let labelNames = ['spo2l', 'hrl', 'sysbpl', 'diabpl', 'waterlevell'];
console.log("reached here, shouldn't reach here");
let TSdata = {
    spo2: [],
    hr: [],
    sysbp: [],
    diabp: [],
    waterlevel: [],
    spo2l: [],
    hrl: [],
    sysbpl: [],
    diabpl: [],
    waterlevell: []
}

// fetch thingspeak data (API key and channel ID) from ./private/Thingspeak.json and assign them to the variables
async function f() {
    console.log("f called");
    await fetch("private/Thingspeak.json").then(response => response.json()).then(data => {
        channelID = data.channelID;
        APIKEY = data.readAPIkey;
        // console.log(data);
    });
    console.log("f is done");
}

async function g() {
    console.log("g called");
    await f();

    for(var numField = 1; numField <= 5; numField++) {
        let TSrequest = "https://api.thingspeak.com/channels/" + channelID + "/fields/" + numField + ".json?api_key=" + APIKEY; // + "&results=" + numResults;
        let response = await fetch(TSrequest);
        let data = await response.json();
        // console.log(data);
        TSdata[fieldNames[numField-1]] = [];
        TSdata[labelNames[numField-1]] = [];
        for(var i = 0; i < 100; i++) {
            if(data.feeds[i]["field" + numField] != null && data.feeds[i]["field" + numField] != '0' && data.feeds[i]["field" + numField] != '0.00') {
                TSdata[fieldNames[numField-1]].push(parseInt(data.feeds[i]["field" + numField]));
                TSdata[labelNames[numField-1]].push("");
            } 
        }
        // console.log(TSdata[fieldNames[numField-1]]);
        // console.log(TSdata[fieldNames[numField-1] + 'l']);

        if(numField == 1) {
            spo2Chart.data.labels = TSdata.spo2l;
            spo2Chart.data.datasets[0].data = TSdata[fieldNames[numField-1]];
            await spo2Chart.update();
        }
        else if(numField == 2) {
            hrChart.data.labels = TSdata.hrl;
            hrChart.data.datasets[0].data = TSdata[fieldNames[numField-1]];
            await hrChart.update();
        }
        else if(numField == 3) {
            bpChart.data.labels = TSdata.sysbpl;
            bpChart.data.datasets[0].data = TSdata[fieldNames[numField-1]];
            await bpChart.update();
        }
        else if(numField == 4) {
            bpChart.data.labels = TSdata.diabpl;
            bpChart.data.datasets[1].data = TSdata[fieldNames[numField-1]];
            await bpChart.update();
        }
        else if(numField == 5) {
            wlChart.data.labels = TSdata.waterlevell;
            wlChart.data.datasets[0].data = TSdata[fieldNames[numField-1]];
            await wlChart.update();
        }
    }

    //truncate sys and dia values
    

    console.log(TSdata);
    console.log("g is done");
}