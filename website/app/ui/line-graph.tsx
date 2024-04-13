"use client"

import React from 'react';
import {
    Chart as ChartJS,
    CategoryScale,
    LinearScale,
    PointElement,
    LineElement,
    Title,
    Tooltip,
    Legend,
} from 'chart.js';
import { Line } from 'react-chartjs-2';

ChartJS.register(
    CategoryScale,
    LinearScale,
    PointElement,
    LineElement,
    Title,
    Tooltip,
    Legend
);

export const options = {
    responsive: true,
    maintainAspectRatio: false,
    plugins: {
        legend: {
            position: 'top' as const,
        },
        title: {
            display: true,
            text: 'Environment Data',
        },
    },
};

export const temps = [
    { "time": "1", "temp": 34.1 },
    { "time": "2", "temp": 36.4 },
    { "time": "3", "temp": 39.1 },
    { "time": "4", "temp": 41.2 },
    { "time": "5", "temp": 45.9 },
    { "time": "6", "temp": 50.5 },
    { "time": "7", "temp": 53.7 },
    { "time": "8", "temp": 60.7 },
    { "time": "9", "temp": 56.9 },
    { "time": "10", "temp": 45.3 },
]

export const data = {
    labels: temps.map(t => t.time),
    datasets: [
        {
            label: 'Temperature',
            data: temps.map(t => t.temp),
            borderColor: 'rgb(255, 99, 132)',
            backgroundColor: 'rgba(255, 99, 132, 0.5)',
        },
    ],
};

export function EnvironmentLineGraph() {
    return <Line options={options} data={data} />;
}
