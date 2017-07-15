# QuCirDET

Qu-antum Cir-cuit D-esign and E-valuator T-ool

QuCirDET is basically an EDA tool for the circuit model of quantum computation, but more software 
modules are going to be added to carry out various simulations.

Circuit Designer:

QuCirDET.pro

main.cpp
mainwindow.cpp
graphwidget.cpp // this is the basic graphical unit where the graphical objects are placed and viewed.
node.cpp // every computing step includes a node which may be a gate or an idle state
edge.cpp // the edge is the line which connects between the nodes 
fnode.cpp // a special case of node - the first node which set different states than gates
dialoguesyndrome.cpp // a dialogue box where the extractor with its ancilla qubits is defined
nodesyndrome.cpp // nodes correlated with the extractor
edgesyndrome.cpp // edges correlated with the extractor

mainwindow.h
graphwidget.h
node.h
edge.h
fnode.h
dialoguesyndrome.h
nodesyndrome.h
edgesyndrome.h

A small qt-database for the managment of the error correction codes is adopted, so it is needed to include the following code:
connection.h

For the construction of plots the following files are also included:
qcustomplot.cpp
qcustomplot.h
