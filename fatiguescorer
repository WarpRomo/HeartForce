function calculateHRV(heartRateData) {
    // Calculate the time differences between consecutive heartbeats
    let rrIntervals = calculateRRIntervals(heartRateData);

    // Calculate the standard deviation of NN intervals (SDNN)
    let sdnn = calculateStandardDeviation(rrIntervals);
    return sdnn;
}

function monitorRestingHeartRate(heartRateData) {
    let restingHR = calculateAverageRestingHR(heartRateData);

    if (restingHR > userBaseline * 1.1) { // 10% increase threshold
        return "Elevated resting heart rate detected. Possible fatigue.";
    }
    return "Resting heart rate within normal range.";
}

function assessHeartRateRecovery(postExerciseData) {
    let recoveryRate = calculateRecoveryRate(postExerciseData);

    if (recoveryRate < userThreshold) {
        return "Slow heart rate recovery detected. Indicates possible fatigue.";
    }
    return "Heart rate recovery within normal range.";
}

function calculateFatigueScore(hrv, restingHR, recoveryRate) {
    let fatigueScore = (hrvWeight * hrv + 
                        restingHRWeight * restingHR + 
                        recoveryRateWeight * recoveryRate) / 
                        (hrvWeight + restingHRWeight + recoveryRateWeight);
    return fatigueScore;
}

function provideFatigueAlert(fatigueScore) {
    if (fatigueScore > highFatigueThreshold) {
        return "High fatigue detected. Recommend immediate rest period.";
    } else if (fatigueScore > moderateFatigueThreshold) {
        return "Moderate fatigue detected. Consider scheduling a rest period soon.";
    }
    return "Fatigue levels within normal range.";
}

