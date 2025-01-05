let throttleGauge, steeringGauge, rotationalRateGauge;

function fetchData() {
    fetch('/data')
        .then(response => response.json())
        .then(data => {
            throttleGauge.refresh(data.throttle);
            steeringGauge.refresh(data.steering);
            rotationalRateGauge.refresh(data.rotational_rate);

            // Update steering mode status indicator
            const steeringStatus = document.getElementById('steering-status');
            steeringStatus.innerText = data.steering_mode;

            // Update drive mode status indicator
            const driveStatus = document.getElementById('drive-status');
            driveStatus.innerText = data.drive_mode;

            // Update motor 1 bars
            document.getElementById('motor1-throttle-bar').style.height = data.motor1_throttle + '%';
            document.getElementById('motor1-throttle-bar').innerText = data.motor1_throttle + '%';
            document.getElementById('motor1-rpm-bar').style.height = (data.motor1_rpm / 100) + '%';
            document.getElementById('motor1-rpm-bar').innerText = data.motor1_rpm + ' RPM';

            // Update motor 2 bars
            document.getElementById('motor2-throttle-bar').style.height = data.motor2_throttle + '%';
            document.getElementById('motor2-throttle-bar').innerText = data.motor2_throttle + '%';
            document.getElementById('motor2-rpm-bar').style.height = (data.motor2_rpm / 100) + '%';
            document.getElementById('motor2-rpm-bar').innerText = data.motor2_rpm + ' RPM';

            // Update motor 3 bars
            document.getElementById('motor3-throttle-bar').style.height = data.motor3_throttle + '%';
            document.getElementById('motor3-throttle-bar').innerText = data.motor3_throttle + '%';
            document.getElementById('motor3-rpm-bar').style.height = (data.motor3_rpm / 100) + '%';
            document.getElementById('motor3-rpm-bar').innerText = data.motor3_rpm + ' RPM';

            // Update motor 4 bars
            document.getElementById('motor4-throttle-bar').style.height = data.motor4_throttle + '%';
            document.getElementById('motor4-throttle-bar').innerText = data.motor4_throttle + '%';
            document.getElementById('motor4-rpm-bar').style.height = (data.motor4_rpm / 100) + '%';
            document.getElementById('motor4-rpm-bar').innerText = data.motor4_rpm + ' RPM';

            // Update G-force dot and values
            const gForceDot = document.getElementById('g-force-dot');
            const gForceContainer = document.getElementById('g-force-container');
            const maxGForce = 10; // Example max G-force value
            const x = (data.g_force_x / maxGForce) * (gForceContainer.clientWidth / 2) + (gForceContainer.clientWidth / 2);
            const y = (data.g_force_y / maxGForce) * (gForceContainer.clientHeight / 2) + (gForceContainer.clientHeight / 2);
            gForceDot.style.left = `${x}px`;
            gForceDot.style.top = `${y}px`;

            document.getElementById('g-force-x-value').innerText = data.g_force_x.toFixed(2);
            document.getElementById('g-force-y-value').innerText = data.g_force_y.toFixed(2);

            // Update arm status indicator
            const armStatus = document.getElementById('arm-status');
            if (data.arm_enabled) {
                armStatus.classList.add('armed');
                armStatus.classList.remove('disarmed');
                armStatus.innerText = 'Armed';
            } else {
                armStatus.classList.add('disarmed');
                armStatus.classList.remove('armed');
                armStatus.innerText = 'Disarmed';
            }
        })
        .catch(error => console.error('Error fetching data:', error));
}

window.onload = function() {
    throttleGauge = new JustGage({
        id: 'throttle-gauge',
        value: 0,
        min: -100,
        max: 100,
        title: 'Throttle'
    });

    steeringGauge = new JustGage({
        id: 'steering-gauge',
        value: 0,
        min: -100,
        max: 100,
        title: 'Steering'
    });

    rotationalRateGauge = new JustGage({
        id: 'rotational-rate-gauge',
        value: 0,
        min: -360,
        max: 360,
        title: 'Rotational Rate'
    });

    setInterval(fetchData, 100); // Fetch data every 100 milliseconds (10 Hz)
};