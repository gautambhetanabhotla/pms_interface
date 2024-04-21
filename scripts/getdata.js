let numResults = 7, numFields = 1;
var APIKEY = null;
var channelID = null;
var TSdata = [[], [], [], [], []];

// fetch thingspeak data (API key and channel ID) from ./private/Thingspeak.json and assign them to the variables
fetch("private/Thingspeak.json").then(response => response.json()).then(data => {
    console.log(data);
    channelID = data["channelID"];
    APIKEY = data["readAPIkey"];
});

for(var numField = 1; numField <= 5; numField++) {
    let TSrequest = "https://api.thingspeak.com/channels/" + channelID + "/fields/" + numField + ".json?api_key=" + APIKEY + "&results=" + numResults;
    fetch(TSrequest).then(response => response.json()).then(data => {
        for(var i = 0; i < numResults; i++) TSdata[numField-1].push(data.feeds[numResults-i-1]);
    });
}