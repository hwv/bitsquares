#!/bin/bash

if [ $# -ne 2 ]; then
echo "Usage: $0 [string length] [number of jobs]"
echo "Creates [number of jobs] job files in directory ./[string length]/"
exit 1;
fi

export N=$1
export BATCHTOTAL=$2

if [ -d "./${N}" ]; then
    echo "Cleaning up..."
    rm -rf "./${N}"
fi

mkdir "${N}"
mkdir "${N}/output"

for BATCHNO in `seq 1 ${BATCHTOTAL}`;
   do
    export BATCHNO
    export PBS_O_WORKDIR='$PBS_O_WORKDIR'
    FILENAME="${N}/job-${N}-${BATCHNO}-${BATCHTOTAL}.sh"
    echo "Creating job: ${FILENAME}"
    envsubst < maxsquares-job.template > $FILENAME
    echo "Submitting job: ${FILENAME}"
    qsub -q compute ${FILENAME}
done 
