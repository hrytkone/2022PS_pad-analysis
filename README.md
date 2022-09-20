# 2022PS_pad-analysis
Codes used to analyse data from 2022 PS test beam for FoCal-E pad layers.

## Electron beam analysis
Codes for electron beam data are included in elecron-analysis

### Usage:
* ./runConstructForAll: runs the analysis for all beam energies used
    * There are two different cases: with and without a box. To change between these two make changes to Reconstruct.h! Change nset (number of data files per energy) and the filenames
    * In Reconstruct.h you can also change the output directory for the data, tag for the files, and the files from which pedestals and MIP calibrations are read from
* ./runSimAnalysisForAll: runs the same analysis but for the simulated data
    * Simulated data (=deposited energy to each layer) has been changed to ADC using the MIP peak values --> noise is added --> saturation in the channels is taken into account --> then convert back to energies
    * The non-modified data is also available in the output

### Plotting:
* PlotClusterObs.C: Plots cluster seed energies and the shower sizes
* PlotShowerComparison.C: Plot energy distribution in each layer, comparison between no box, box, and simulated cases
* PlotEnergyResolution.C: Plot energy distribution summed from all the layers, fit these for energy resolution plot
