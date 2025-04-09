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
    TimeScale,
} from 'chart.js';
import 'chartjs-adapter-luxon'
import { Line } from 'react-chartjs-2';
import { DateTime } from 'luxon';

ChartJS.register(
    CategoryScale,
    LinearScale,
    PointElement,
    LineElement,
    TimeScale,
    Title,
    Tooltip,
    Legend
);

export default function EnvironmentLineGraph({ xlabels, label, title, color, backgroundColor, d }: { xlabels: (string | null)[], label: string, title: string, color: string, backgroundColor: string, d: any }) {

    let data = {
        labels: xlabels,
        datasets: [
            {
                label: label,
                data: d,
                borderColor: color,
                backgroundColor: backgroundColor,
                stepped: false,
            }
        ]
    }

    let options: any = {
        scales: {
            x: {
                type: 'time',
                time: {
                    unit: 'minute',
                    unitStepSize: 1,
                    displayFormats: {
                        'minute': 'hh:mm a'
                    }
                },
            }
        },
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
