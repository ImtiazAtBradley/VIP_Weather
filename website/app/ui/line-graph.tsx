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

export default function EnvironmentLineGraph({ label, title, color, backgroundColor, d }: { label: string, title: string, color: string, backgroundColor: string, d: { time: string, val: number }[] }) {

    let data = {
        labels: d.map(x => x.time),
        datasets: [
            {
                label: label,
                data: d.map(x => x.val),
                borderColor: color,
                backgroundColor: backgroundColor,
            }
        ]
    }

    let options = {
        responsive: true,
        maintainAspectRatio: false,
        plugins: {
            legend: {
                position: 'top' as const,
            },
            title: {
                display: true,
                text: title,
            },
        },
    }

    return (
        <div className='flex h-96'>
            <Line options={options} data={data} />
        </div>
    )
}
