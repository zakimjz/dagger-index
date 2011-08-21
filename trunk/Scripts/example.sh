#!/bin/sh

sh runMethod.sh ../Data/ba1Mk8.gra ../Data/ba1Mk8.test
sh extractTotalTime.sh ba1Mk8
sh qpuPlotbw.sh ba1Mk8.tt 8

sh runMethod.sh ../Data/ba1Mk2bidir.gra ../Data/ba1Mk2bidir.test
sh extractTotalTime.sh ba1Mk2bidir
sh qpuPlotbw.sh ba1Mk2bidir.tt 8

sh runMethod.sh ../Data/er1Mk2.gra ../Data/er1Mk2.test
sh extractTotalTime.sh er1Mk2
sh qpuPlotbw.sh er1Mk2.tt 8
