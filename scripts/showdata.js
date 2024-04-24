Chart.defaults.scales.category.grid = {
    display: false
};

var hrdata = {
    labels: TSdata.hrl,
    datasets: [{
        label: 'Heart rate',
        data: TSdata.hr,
        fill: false,
        borderColor: 'rgb(255, 0, 0)',
        tension: 0
    }]
};

var bpdata = {
    labels: TSdata.sysbpl,
    datasets: [
        {
            label: 'Systolic BP',
            data: TSdata.sysbp,
            fill: false,
            borderColor: 'rgb(161, 3, 252)',
            tension: 0
        },
        {
            label: 'Diastolic BP',
            data: TSdata.diabp,
            fill: false,
            borderColor: 'rgb(252, 3, 169)',
            tension: 0
        }
    ]
};

var spo2data = {
    labels: TSdata.spo2l,
    datasets: [
        {
            label: 'SpO2',
            data: TSdata.spo2,
            fill: false,
            borderColor: 'rgb(3, 252, 44)',
            tension: 0
        }
    ]
};

var waterleveldata = {
    labels: TSdata.waterlevell,
    datasets: [
        {
            label: 'Water level',
            data: TSdata.waterlevel,
            fill: false,
            borderColor: 'rgb(0, 236, 255)',
            tension: 0
        }
    ]
};

var hr = document.getElementById("heartRate");
var hrChart = new Chart(hr, {
    type: 'line',
    data: hrdata,
    options: {
        plugins: {
            legend: {
                labels: {
                    color: 'rgb(255, 255, 255)'
                }
            }
        },
        scales: {
            y: {
                ticks: {
                    min: 60,
                    max: 140,
                }
            }
        },
        title: {
            display: true,
            text: 'Heart rate'
        }
    }
});

var bp = document.getElementById("BP");
var bpChart = new Chart(bp, {
    type: 'line',
    data: bpdata,
    options: {
        plugins: {
            legend: {
                labels: {
                    color: 'rgb(255, 255, 255)'
                }
            }
        }
    }
});

var _spo2 = document.getElementById("Spo2");
var spo2Chart = new Chart(_spo2, {
    type: 'line',
    data: spo2data,
    options: {
        plugins: {
            legend: {
                labels: {
                    color: 'rgb(255, 255, 255)'
                }
            }
        }
    }
});

var wl = document.getElementById("waterLevel");
var wlChart = new Chart(wl, {
    type: 'line',
    data: waterleveldata,
    options: {
        plugins: {
            legend: {
                labels: {
                    color: 'rgb(255, 255, 255)'
                }
            }
        }
    }
});

async function updateCharts() {
    console.log("UPDATING CHARTS");
    await g();
}

updateCharts();
setInterval(updateCharts, 15000);