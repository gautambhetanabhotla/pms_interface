function average(array) {
    var sum = 0;
    for(var i = 0; i < array.length; i++) {
        sum += array[i];
    }
    var ans = sum/array.length
    return ans.toFixed(2);
}

function current(array) {
    return array[array.length - 1];
}

var avg_hr = average(hrChart.data.datasets[0].data);
var avg_sysbp = average(bpChart.data.datasets[0].data);
var avg_diabp = average(bpChart.data.datasets[1].data);
var avg_wl = average(wlChart.data.datasets[0].data);
var avg_spo2 = average(spo2Chart.data.datasets[0].data);

var current_hr = current(hrChart.data.datasets[0].data);
var current_sysbp = current(bpChart.data.datasets[0].data);
var current_diabp = current(bpChart.data.datasets[1].data);
var current_wl = current(wlChart.data.datasets[0].data);
var current_spo2 = current(spo2Chart.data.datasets[0].data);

var hrc = document.getElementById("hrc");
var hra = document.getElementById("hra");
var bpsc = document.getElementById("bpsc");
var bpsa = document.getElementById("bpsa");
var bpdc = document.getElementById("bpdc");
var bpda = document.getElementById("bpda");
var spo2c = document.getElementById("spo2c");
var spo2a = document.getElementById("spo2a");
var wlc = document.getElementById("wlc");
var wla = document.getElementById("wla");

async function updateAnalytics() {
    await updateCharts();

    var avg_hr = average(hrChart.data.datasets[0].data);
    var avg_sysbp = average(bpChart.data.datasets[0].data);
    var avg_diabp = average(bpChart.data.datasets[1].data);
    var avg_wl = average(wlChart.data.datasets[0].data);
    var avg_spo2 = average(spo2Chart.data.datasets[0].data);

    var current_hr = current(hrChart.data.datasets[0].data);
    var current_sysbp = current(bpChart.data.datasets[0].data);
    var current_diabp = current(bpChart.data.datasets[1].data);
    var current_wl = current(wlChart.data.datasets[0].data);
    var current_spo2 = current(spo2Chart.data.datasets[0].data);

    hrc.innerHTML = "Current: " + current_hr + "bpm";
    hra.innerHTML = "Average: " + avg_hr + "bpm";
    bpsc.innerHTML = "Current: " + current_sysbp + "mmHg";
    bpsa.innerHTML = "Average: " + avg_sysbp + "mmHg";
    bpdc.innerHTML = "Current: " + current_diabp + "mmHg";
    bpda.innerHTML = "Average: " + avg_diabp + "mmHg";
    spo2c.innerHTML = "Current: " + current_spo2 + "%";
    spo2a.innerHTML = "Average: " + avg_spo2 + "%";
    wlc.innerHTML = "Current: " + current_wl + "cm";
    wla.innerHTML = "Average: " + avg_wl + "cm";
}

updateAnalytics();
setInterval(updateAnalytics, 15000);