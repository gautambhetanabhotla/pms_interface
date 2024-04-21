var labels = [];
for(var i = 0; i < numResults; i++) {
    labels.push("");
}

var data = [65, 59, 80, 81, 56, 55, 40];

console.log(labels);

var hrdata = {
    labels: labels,
    datasets: [{
        label: 'Heart rate',
        data: data,
        fill: false,
        borderColor: 'rgb(0, 236, 169)',
        tension: 0
    }]
};

var hr = document.getElementById("heartRate");
new Chart(hr, {
    type: 'line',
    data: hrdata,
    options: {
        plugins: {
            legend: {
                labels: {
                    color: 'rgb(255, 255, 255)' // Change the color to red
                }
            },
            title: {
                display: false,
                text: 'IOTIOTIOTIOT',
                color: 'rgb(255, 0, 0)' // Change the color to red
            },
            scales: {
                x: {
                    ticks: {
                        display: false,
                        color: 'rgb(255, 0, 0)' // Change the color to red
                    }
                },
                y: {
                    ticks: {
                        display: false,
                        color: 'rgb(255, 0, 0)' // Change the color to red
                    }
                }
            }
        }
    }
});

var bp = document.getElementById("BP");
new Chart(bp, {
    type: 'line',
    data: hrdata
})