#! /bin/bash
gnome-terminal -- ./DCmotor.exe 1.8 0.02 0.005 0.025 5e-5 6e-6 0.01 L
gnome-terminal -- ./DCmotor.exe 1.8 0.02 0.005 0.025 5e-5 6e-6 0.01 R
gnome-terminal -- ./RegPID.exe 0.02 16 0 0.01 L
gnome-terminal -- ./RegPID.exe 0.02 16 0 0.01 R
gnome-terminal -- ./Kinematics.exe 0.1 0.4 0.01
gnome-terminal -- ./Odometry.exe 0.01 0 0 0

