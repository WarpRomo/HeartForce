

# HeartForce⚡
HeartForce⚡ provides real-time monitoring of heart rate and oxygen saturation levels for individuals wearing the HeartForce wristband. This dashboard offers instant alerts for potential heart health risks, detailed heart history analysis, and adjustable thresholds to cater to each user’s needs.

<h1><a id="title" href="https://heartforce.live" target="_blank">https://heartforce.live</a></h1>

<img src="./readme/heartforce_dashboard.png"></img>
<img src="./readme/heartforce_alert.png"></img>

---

### Features

- **Real-Time Monitoring**: Displays current heart rate (BPM) and oxygen saturation (SpO₂) for all active users.
- **Customizable Thresholds**: Allows users to adjust thresholds for cardiac risk (BPM) and low oxygen levels (SpO₂).
- **Heart History Analysis**: Enables users to access past records of an individual’s heart performance.
- **HeartAI Assistant**: An AI assistant that responds to queries regarding the patient’s heart health and fitness levels.
- **Responsive Design**: Adaptable for use on desktops, tablets, and mobile devices.

---

### Code Structure

- **index.html**: Contains the layout and HTML structure of the HeartForce dashboard.
- **script.js**: JavaScript file for handling data visualization, threshold adjustments, and AI interaction.
- **server.js**: Express server setup with Socket.io integration for real-time data transfer between backend and frontend.
- **styles.css**: Styling for UI elements and overall design consistency.

---

### Usage

1. **Adjust Health Thresholds**  
   - Use sliders on the main dashboard to set personalized heart and oxygen risk thresholds.
   
2. **View Patient History**  
   - Select a patient to view detailed metrics on heart rate, oxygen levels, and fatigue scores.

3. **AI Health Assistant**  
   - Ask the HeartAI assistant specific questions about a patient’s heart health for real-time insights.

---

### Installation

1. **Clone the Repository**  
   ```bash
   git clone <repository-url>
   cd HeartForce
   ```

2. **Install Dependencies**  
   ```bash
   npm install express socket.io cors
   ```

3. **Run the Server**  
   ```bash
   node server.js
   ```

4. **Access the Dashboard**  
   - Open your browser to [localhost:8080](http://localhost:8080) to access the HeartForce dashboard.

---

### Future Enhancements

- Integration with medical device APIs for continuous monitoring.
- Mobile app compatibility for remote health monitoring.

