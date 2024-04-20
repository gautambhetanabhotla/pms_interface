let numResults = 7, numFields = 1;
var APIKEY = null;
var channelID = null;
let TSrequest = "https://api.thingspeak.com/channels/" + channelID + "/feeds.json?api_key=" + APIKEY + "&results=" + numResults;

// fetch thingspeak data (API key and channel ID) from ./private/Thingspeak.json and assign them to the variables
fetch("private/Thingspeak.json").then(response => response.json()).then(data => {
    console.log(data);
    channelID = data["channelID"];
    APIKEY = data["readAPIkey"];
});