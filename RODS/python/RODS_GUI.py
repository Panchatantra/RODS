# -*- coding: utf-8 -*-

import sys
from PySide6.QtCore import Qt, Slot
from PySide6.QtGui import QColor, QIcon, QPainter, QPen, QAction
from PySide6.QtWidgets import QApplication, QWidget, QMainWindow, QTableWidget, QTableWidgetItem, \
                            QPushButton, QHBoxLayout, QVBoxLayout, QGridLayout, \
                            QLabel, QSpinBox, QDoubleSpinBox, QComboBox, QLineEdit, QFileDialog
                            
import numpy as np
import pickle

import matplotlib
matplotlib.use("Qt5Agg")
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

from RODS_model import *


class PlotWidget(FigureCanvas):
    
    def __init__(self):
        self.fig = Figure()
        self.fig.subplots_adjust(left=0.0, bottom=0.0, right=1.0, top=1.0, wspace=0.0, hspace=0.0)
        super().__init__(self.fig)
        self.axes = self.fig.add_subplot(111)


class AddDOFWidget(QWidget):
    """AddDOFWidget"""
    def __init__(self):
        super(AddDOFWidget, self).__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Add DOF')
        self.setWindowIcon(QIcon("RODS.ico"))
        
        idLabel = QLabel("ID")
        self.id = QSpinBox()
        dirLabel = QLabel("Direction")
        self.dir = QComboBox()
        for dir in Direction:
            self.dir.addItem(dir.name)
        massLabel = QLabel("mass")
        self.mass = QDoubleSpinBox()

        grid = QGridLayout()
        grid.addWidget(idLabel,0,0)
        grid.addWidget(self.id,0,1)
        grid.addWidget(dirLabel,1,0)
        grid.addWidget(self.dir,1,1)
        grid.addWidget(massLabel,2,0)
        grid.addWidget(self.mass,2,1)

        self.addButton = QPushButton("Add")
        cancelButton = QPushButton("Cancel")

        cancelButton.clicked.connect(self.close)

        btnBox = QHBoxLayout()
        btnBox.addWidget(self.addButton)
        btnBox.addWidget(cancelButton)

        vBox = QVBoxLayout()
        vBox.addLayout(grid)
        vBox.addLayout(btnBox)

        self.setLayout(vBox)


class AddWaveWidget(QWidget):
    """AddWaveWidget"""
    def __init__(self):
        super(AddWaveWidget, self).__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Add Wave')
        self.setWindowIcon(QIcon("RODS.ico"))

        idLabel = QLabel("ID")
        self.id = QSpinBox()
        dtLabel = QLabel("Δt")
        self.dt = QDoubleSpinBox()
        self.dt.setRange(0.0,10.0)
        self.dt.setValue(0.02)
        self.dt.setDecimals(3)
        fnLabel = QLabel("Wave file")
        self.fn = QLineEdit()
        self.fnb = QPushButton("...")
        self.fnb.clicked.connect(self.getWaveFileName)

        grid = QGridLayout()
        grid.addWidget(idLabel,0,0)
        grid.addWidget(self.id,0,1)
        grid.addWidget(dtLabel,1,0)
        grid.addWidget(self.dt,1,1)
        grid.addWidget(fnLabel,2,0)
        grid.addWidget(self.fn,2,1)
        grid.addWidget(self.fnb,2,2)

        self.addButton = QPushButton("Add")
        cancelButton = QPushButton("Cancel")

        cancelButton.clicked.connect(self.close)

        self.plotButton = QPushButton("Plot")
        btnBox = QHBoxLayout()
        btnBox.addWidget(self.addButton)
        btnBox.addWidget(self.plotButton)
        btnBox.addWidget(cancelButton)
        
        self.plotButton.clicked.connect(self.plotWave)
        
        self.canvas = PlotWidget()
        self.canvas.fig.set_figheight(self.canvas.fig.get_figheight()*0.7)

        vBox = QVBoxLayout()
        vBox.addLayout(grid)
        vBox.addLayout(btnBox)
        vBox.addWidget(self.canvas)

        self.setLayout(vBox)
        
    def getWaveFileName(self):
        fn, _ = QFileDialog.getOpenFileName(self,
                "getWaveFileName","./",
                "All Files (*);;Text Files (*.txt)")
        self.fn.setText(fn)
        
    def plotWave(self):
        fn = self.fn.text()
        dt = self.dt.value()
        data = np.loadtxt(fn)
        n = len(data)
        t = np.linspace(0, dt*(n-1), n)
        self.canvas.axes.cla()
        self.canvas.axes.plot(t,data)
        self.canvas.axes.set_xlabel("Time")
        self.canvas.axes.set_ylabel("Acceleration")
        self.canvas.axes.grid(True)
        self.canvas.draw()


class RemoveDOFWidget(QWidget):
    """RemoveDOFWidget"""
    def __init__(self):
        super(RemoveDOFWidget, self).__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Remove DOF')
        self.setWindowIcon(QIcon("RODS.ico"))

        idLabel = QLabel("ID")
        self.id = QComboBox()

        grid = QGridLayout()
        grid.addWidget(idLabel,0,0)
        grid.addWidget(self.id,0,1)

        self.removeButton = QPushButton("Remove")
        cancelButton = QPushButton("Cancel")

        cancelButton.clicked.connect(self.close)

        btnBox = QHBoxLayout()
        btnBox.addWidget(self.removeButton)
        btnBox.addWidget(cancelButton)

        vBox = QVBoxLayout()
        vBox.addLayout(grid)
        vBox.addLayout(btnBox)

        self.setLayout(vBox)

    def fillComboBox(self, IDs, Show=True):
        strIDs = [str(id) for id in IDs]
        self.id.clear()
        self.id.addItems(strIDs)
        if Show:
            self.show()


class FixDOFWidget(QWidget):
    """FixDOFWidget"""
    def __init__(self):
        super(FixDOFWidget, self).__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Fix DOF')
        self.setWindowIcon(QIcon("RODS.ico"))

        idLabel = QLabel("ID")
        self.id = QComboBox()

        grid = QGridLayout()
        grid.addWidget(idLabel,0,0)
        grid.addWidget(self.id,0,1)

        self.fixButton = QPushButton("Fix")
        self.freeButton = QPushButton("Free")
        cancelButton = QPushButton("Cancel")
        cancelButton.clicked.connect(self.close)

        btnBox = QHBoxLayout()
        btnBox.addWidget(self.fixButton)
        btnBox.addWidget(self.freeButton)
        btnBox.addWidget(cancelButton)

        vBox = QVBoxLayout()
        vBox.addLayout(grid)
        vBox.addLayout(btnBox)

        self.setLayout(vBox)

    def fillComboBox(self, IDs, Show=True):
        strIDs = [str(id) for id in IDs]
        self.id.clear()
        self.id.addItems(strIDs)
        if Show:
            self.show()


class DOFTableWidget(QWidget):
    """DOFTableWidget"""
    def __init__(self):
        super(DOFTableWidget, self).__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('DOF')
        self.setWindowIcon(QIcon("RODS.ico"))

        self.table = QTableWidget()
        self.table.setColumnCount(4)
        self.table.setHorizontalHeaderLabels(['ID','Direction','Mass','isFixed'])

        vBox = QVBoxLayout()
        vBox.addWidget(self.table)

        self.setLayout(vBox)

    def fillTable(self, DOFs, show=False):
        self.table.clearContents()
        self.table.setRowCount(len(DOFs))
        i = 0
        for id in DOFs.keys():
            dof = DOFs[id]
            newItem = QTableWidgetItem(str(dof.id))
            newItem.setFlags(newItem.flags() & ~Qt.ItemIsEditable)
            self.table.setItem(i,0,newItem)
            newItem = QTableWidgetItem(dof.dir.name)
            newItem.setFlags(newItem.flags() & ~Qt.ItemIsEditable)
            self.table.setItem(i,1,newItem)
            newItem = QTableWidgetItem(str(dof.mass))
            newItem.setFlags(newItem.flags() & ~Qt.ItemIsEditable)
            self.table.setItem(i,2,newItem)
            newItem = QTableWidgetItem(str(dof.isFixed))
            newItem.setFlags(newItem.flags() & ~Qt.ItemIsEditable)
            self.table.setItem(i,3,newItem)

            i += 1
        
        if show:
            self.show()

class WaveTableWidget(QWidget):
    """WaveTableWidget"""
    def __init__(self):
        super(WaveTableWidget, self).__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Wave')
        self.setWindowIcon(QIcon("RODS.ico"))

        self.table = QTableWidget()
        self.table.setColumnCount(3)
        self.table.setHorizontalHeaderLabels(['ID','Name','dt'])

        vBox = QVBoxLayout()
        vBox.addWidget(self.table)

        self.setLayout(vBox)

    def fillTable(self, Waves, show=False):
        self.table.clearContents()
        self.table.setRowCount(len(Waves))
        i = 0
        for id in Waves.keys():
            wave = Waves[id]
            newItem = QTableWidgetItem(str(wave.id))
            newItem.setFlags(newItem.flags() & ~Qt.ItemIsEditable)
            self.table.setItem(i,0,newItem)
            newItem = QTableWidgetItem(wave.name)
            newItem.setFlags(newItem.flags() & ~Qt.ItemIsEditable)
            self.table.setItem(i,1,newItem)
            newItem = QTableWidgetItem(str(wave.dt))
            newItem.setFlags(newItem.flags() & ~Qt.ItemIsEditable)
            self.table.setItem(i,2,newItem)

            i += 1

        if show:
            self.show()


class Link1DTableWidget(QWidget):
    """Link1DTableWidget"""
    def __init__(self):
        super(Link1DTableWidget, self).__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Link1D')
        self.setWindowIcon(QIcon("RODS.ico"))

        self.table = QTableWidget()
        self.table.setColumnCount(5)
        self.table.setHorizontalHeaderLabels(['ID','DOF I','DOF J','Type','Param'])

        vBox = QVBoxLayout()
        vBox.addWidget(self.table)

        self.setLayout(vBox)

    def fillTable(self, Link1Ds):
        self.table.clearContents()
        self.table.setRowCount(len(Link1Ds))
        i = 0
        for id in Link1Ds.keys():
            link1d = Link1Ds[id]
            newItem = QTableWidgetItem(str(link1d.id))
            newItem.setFlags(newItem.flags() & ~Qt.ItemIsEditable)
            self.table.setItem(i,0,newItem)
            newItem = QTableWidgetItem(str(link1d.dofI))
            newItem.setFlags(newItem.flags() & ~Qt.ItemIsEditable)
            self.table.setItem(i,1,newItem)
            newItem = QTableWidgetItem(str(link1d.dofJ))
            newItem.setFlags(newItem.flags() & ~Qt.ItemIsEditable)
            self.table.setItem(i,2,newItem)
            newItem = QTableWidgetItem(str(link1d.type.name))
            newItem.setFlags(newItem.flags() & ~Qt.ItemIsEditable)
            self.table.setItem(i,3,newItem)
            params = ""
            for p in link1d.params:
                params += str(p)
                params += "; "
            newItem = QTableWidgetItem(params)
            newItem.setFlags(newItem.flags() & ~Qt.ItemIsEditable)
            self.table.setItem(i,4,newItem)

            i += 1


class AddSpringWidget(QWidget):
    """AddSpringWidget"""
    def __init__(self):
        super(AddSpringWidget, self).__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Add Spring')
        self.setWindowIcon(QIcon("RODS.ico"))

        idLabel = QLabel("ID")
        self.id = QSpinBox()
        dofILabel = QLabel("DOF I")
        self.dofI = QComboBox()
        dofJLabel = QLabel("DOF J")
        self.dofJ = QComboBox()

        parameterLabel = QLabel("Stiffness")
        self.parameter = QDoubleSpinBox()
        self.parameter.setMinimum(0.0)
        self.parameter.setMaximum(1.0e20)

        grid = QGridLayout()
        grid.addWidget(idLabel,0,0)
        grid.addWidget(self.id,0,1)
        grid.addWidget(dofILabel,1,0)
        grid.addWidget(self.dofI,1,1)
        grid.addWidget(dofJLabel,2,0)
        grid.addWidget(self.dofJ,2,1)
        grid.addWidget(parameterLabel,3,0)
        grid.addWidget(self.parameter,3,1)

        self.addButton = QPushButton("Add")
        cancelButton = QPushButton("Cancel")

        cancelButton.clicked.connect(self.close)

        btnBox = QHBoxLayout()
        btnBox.addWidget(self.addButton)
        btnBox.addWidget(cancelButton)

        vBox = QVBoxLayout()
        vBox.addLayout(grid)
        vBox.addLayout(btnBox)

        self.setLayout(vBox)

    def fillComboBox(self, IDs, Show=True):
        strIDs = [str(id) for id in IDs]
        self.dofI.clear()
        self.dofI.addItems(strIDs)
        self.dofJ.clear()
        self.dofJ.addItems(strIDs)
        if Show:
            self.show()


class AddDashpotWidget(QWidget):
    """AddDashpotWidget"""
    def __init__(self):
        super(AddDashpotWidget, self).__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('Add Dashpot')
        self.setWindowIcon(QIcon("RODS.ico"))

        idLabel = QLabel("ID")
        self.id = QSpinBox()
        dofILabel = QLabel("DOF I")
        self.dofI = QComboBox()
        dofJLabel = QLabel("DOF J")
        self.dofJ = QComboBox()

        parameterLabel = QLabel("Damping coefficient")
        self.parameter = QDoubleSpinBox()
        self.parameter.setMinimum(0.0)
        self.parameter.setMaximum(1.0e20)

        grid = QGridLayout()
        grid.addWidget(idLabel,0,0)
        grid.addWidget(self.id,0,1)
        grid.addWidget(dofILabel,1,0)
        grid.addWidget(self.dofI,1,1)
        grid.addWidget(dofJLabel,2,0)
        grid.addWidget(self.dofJ,2,1)
        grid.addWidget(parameterLabel,3,0)
        grid.addWidget(self.parameter,3,1)

        self.addButton = QPushButton("Add")
        cancelButton = QPushButton("Cancel")

        cancelButton.clicked.connect(self.close)

        btnBox = QHBoxLayout()
        btnBox.addWidget(self.addButton)
        btnBox.addWidget(cancelButton)

        vBox = QVBoxLayout()
        vBox.addLayout(grid)
        vBox.addLayout(btnBox)

        self.setLayout(vBox)

    def fillComboBox(self, IDs, Show=True):
        strIDs = [str(id) for id in IDs]
        self.dofI.clear()
        self.dofI.addItems(strIDs)
        self.dofJ.clear()
        self.dofJ.addItems(strIDs)
        if Show:
            self.show()


class MainWindow(QMainWindow):

    def __init__(self):
        super().__init__()
        self.model = Model()
        self.initWidgets()
        self.initUI()

    def initWidgets(self):
        self.addDOFWidget = AddDOFWidget()
        self.addDOFWidget.addButton.clicked.connect(self.addDOF)

        self.fixDOFWidget = FixDOFWidget()
        self.fixDOFWidget.fixButton.clicked.connect(self.fixDOF)
        self.fixDOFWidget.freeButton.clicked.connect(self.freeDOF)

        self.removeDOFWidget = RemoveDOFWidget()
        self.removeDOFWidget.removeButton.clicked.connect(self.removeDOF)

        self.DOFTableWidget = DOFTableWidget()

        self.addSpringWidget = AddSpringWidget()
        self.addSpringWidget.addButton.clicked.connect(self.addSpring)
        self.addDashpotWidget = AddDashpotWidget()
        self.addDashpotWidget.addButton.clicked.connect(self.addDashpot)
        self.Link1DTableWidget = Link1DTableWidget()
        
        self.addWaveWidget = AddWaveWidget()
        self.addWaveWidget.addButton.clicked.connect(self.addWave)
        self.WaveTableWidget = WaveTableWidget()
        
    def initUI(self):
        self.setGeometry(300, 100, 900, 300)
        self.statusBar().showMessage('Welcome to use RODS!')
        self.setWindowTitle('RODS')
        self.setWindowIcon(QIcon("RODS.ico"))

        menubar = self.menuBar()

        # File Menu
        newModelAction = QAction('New', self)
        newModelAction.setShortcut('Ctrl+N')
        newModelAction.setStatusTip('Build a new structral model')
        openModelAction = QAction('Open', self)
        openModelAction.setShortcut('Ctrl+O')
        openModelAction.setStatusTip('Open a structral model')
        openModelAction.triggered.connect(self.open)
        
        saveModelAction = QAction('Save', self)
        saveModelAction.setShortcut('Ctrl+S')
        saveModelAction.setStatusTip('Save the structral model')
        saveModelAction.triggered.connect(self.save)

        exitAction = QAction('Exit', self)
        exitAction.setShortcut('Ctrl+Q')
        exitAction.setStatusTip('Exit RODS')
        exitAction.triggered.connect(self.close)

        fileMenu = menubar.addMenu('&File')
        fileMenu.addAction(newModelAction)
        fileMenu.addAction(saveModelAction)
        fileMenu.addAction(openModelAction)
        fileMenu.addAction(exitAction)

        # DOF Menu
        addDOFAction = QAction('Add DOF', self)
        addDOFAction.setStatusTip('Add a new DOF')
        addDOFAction.triggered.connect(self.addDOFWidget.show)

        fixDOFAction = QAction('Fix/Free DOF', self)
        fixDOFAction.setStatusTip('Fix/Free a DOF')
        fixDOFAction.triggered.connect(lambda: self.fixDOFWidget.fillComboBox(self.model.DOFs.keys()))

        removeDOFAction = QAction('Remove DOF', self)
        removeDOFAction.setStatusTip('Remove a DOF')
        removeDOFAction.triggered.connect(lambda: self.removeDOFWidget.fillComboBox(self.model.DOFs.keys()))

        DOFTableAction = QAction('DOF Table', self)
        DOFTableAction.setStatusTip('View DOF Table')
        DOFTableAction.triggered.connect(lambda: self.DOFTableWidget.fillTable(self.model.DOFs, True))
        
        dofMenu = menubar.addMenu('&DOF')
        dofMenu.addAction(addDOFAction)
        dofMenu.addAction(fixDOFAction)
        dofMenu.addAction(removeDOFAction)
        dofMenu.addAction(DOFTableAction)

        # Material1D Menu
        materialMenu = menubar.addMenu('&Material')

        addElastoplasticAction = QAction('Add Elastoplastic', self)
        addElastoplasticAction.setStatusTip('Add a new Elastoplastic Material')
        addConcreteTrilinearAction = QAction('Add ConcreteTrilinear', self)
        addConcreteTrilinearAction.setStatusTip('Add a new ConcreteTrilinear Material')
        addSteelBilinearAction = QAction('Add SteelBilinear', self)
        addSteelBilinearAction.setStatusTip('Add a new SteelBilinear Material')
        addSMABilinearAction = QAction('Add SMABilinear', self)
        addSMABilinearAction.setStatusTip('Add a new SMABilinear Material')

        material1DMenu = materialMenu.addMenu('&Material1D')
        material1DMenu.addAction(addElastoplasticAction)
        material1DMenu.addAction(addConcreteTrilinearAction)
        material1DMenu.addAction(addSteelBilinearAction)
        material1DMenu.addAction(addSMABilinearAction)

        # Link Menu
        linkMenu = menubar.addMenu('&Link')

        ## Link1D Menu
        addSpringAction = QAction('Add Spring', self)
        addSpringAction.setStatusTip('Add a new Spring')
        addSpringAction.triggered.connect(lambda: self.addSpringWidget.fillComboBox(self.model.DOFs.keys()))

        addDashpotAction = QAction('Add Dashpot', self)
        addDashpotAction.setStatusTip('Add a new Dashpot')
        addDashpotAction.triggered.connect(lambda: self.addDashpotWidget.fillComboBox(self.model.DOFs.keys()))
        
        addInerterAction = QAction('Add Inerter', self)
        addInerterAction.setStatusTip('Add a new Inerter')

        addSpringBilinearAction = QAction('Add SpringBilinear', self)
        addSpringBilinearAction.setStatusTip('Add a new SpringBilinear')
        addSpringNonlinearAction = QAction('Add SpringNonlinear', self)
        addSpringNonlinearAction.setStatusTip('Add a new SpringNonlinear')

        link1DMenu = linkMenu.addMenu('&Link1D')
        link1DMenu.addAction(addSpringAction)
        link1DMenu.addAction(addDashpotAction)
        link1DMenu.addAction(addInerterAction)
        link1DMenu.addAction(addSpringBilinearAction)
        link1DMenu.addAction(addSpringNonlinearAction)

        ## Link2D Menu
        addSpring2DAction = QAction('Add Spring2D', self)
        addSpring2DAction.setStatusTip('Add a new Spring2D element')
        addDashpot2DAction = QAction('Add Dashpot2D', self)
        addDashpot2DAction.setStatusTip('Add a new Dashpot2D element')
        addInerter2DAction = QAction('Add Inerter2D', self)
        addInerter2DAction.setStatusTip('Add a new Inerter2D element')

        link2DMenu = linkMenu.addMenu('&Link2D')
        link2DMenu.addAction(addSpring2DAction)
        link2DMenu.addAction(addDashpot2DAction)
        link2DMenu.addAction(addInerter2DAction)

        # Node Menu
        addNodeAction = QAction('Add Node', self)
        addNodeAction.setStatusTip('Add a new Node')
        fixNodeAction = QAction('Fix Node', self)
        fixNodeAction.setStatusTip('Fix a Node')

        nodeMenu = menubar.addMenu('&Node')
        nodeMenu.addAction(addNodeAction)
        nodeMenu.addAction(fixNodeAction)

        # Load Menu
        addNodalLoadAction = QAction('Add Nodal Load', self)
        addNodalLoadAction.setStatusTip('Add a new Nodal Load')
        addWaveAction = QAction('Add Wave', self)
        addWaveAction.setStatusTip('Add a new Wave')
        addWaveAction.triggered.connect(self.addWaveWidget.show)

        activateGroundMotionAction = QAction('Activate Ground Motion', self)
        activateGroundMotionAction.setStatusTip('Activate Ground Motion')

        loadMenu = menubar.addMenu('&Load')
        loadMenu.addAction(addNodalLoadAction)
        loadMenu.addAction(addWaveAction)
        loadMenu.addAction(activateGroundMotionAction)

        # Element Menu
        addTrussElastic2DAction = QAction('Add TrussElastic2D', self)
        addTrussElastic2DAction.setStatusTip('Add a new TrussElastic2D element')
        addFrameElastic2DAction = QAction('Add FrameElastic2D', self)
        addFrameElastic2DAction.setStatusTip('Add a new FrameElastic2D element')
        addQuad4ElasticAction = QAction('Add Quad4Elastic', self)
        addQuad4ElasticAction.setStatusTip('Add a new Quad4Elastic element')

        elementMenu = menubar.addMenu('&Element')
        elementMenu.addAction(addTrussElastic2DAction)
        elementMenu.addAction(addFrameElastic2DAction)
        elementMenu.addAction(addQuad4ElasticAction)

        # Recorder Menu
        addDOFRecoderAction = QAction('Add DOF Recoder', self)
        addDOFRecoderAction.setStatusTip('Add a new DOF Recoder')
        addElementRecoderAction = QAction('Add Element Recoder', self)
        addElementRecoderAction.setStatusTip('Add a new Element Recoder')

        recorderMenu = menubar.addMenu('&Recorder')
        recorderMenu.addAction(addDOFRecoderAction)
        recorderMenu.addAction(addElementRecoderAction)

        # Solve Menu
        solveMenu = menubar.addMenu('&Solve')

        buildModelAction = QAction('Build Model', self)
        buildModelAction.setStatusTip('Build Model')
        buildModelAction.triggered.connect(self.buildModel)
        
        ## Option Menu
        setEigenSolverAction = QAction('Set Eigen', self)
        setEigenSolverAction.setStatusTip('Set Options for Eigen analysis')
        setStaticSolverAction = QAction('Set Static', self)
        setStaticSolverAction.setStatusTip('Set Options for Static analysis')
        setDynamicSolverAction = QAction('Set Dynamic', self)
        setDynamicSolverAction.setStatusTip('Set Options for Dynamic analysis')

        optionMenu = solveMenu.addMenu('&Option')
        optionMenu.addAction(setEigenSolverAction)
        optionMenu.addAction(setStaticSolverAction)
        optionMenu.addAction(setDynamicSolverAction)

        solveEigenAction = QAction('Eigen', self)
        solveEigenAction.setStatusTip('Execute Eigen analysis')
        solveEigenAction.triggered.connect(self.solveEigen)
        
        solveStaticAction = QAction('Static', self)
        solveStaticAction.setStatusTip('Execute Static analysis')
        solveDynamicAction = QAction('Dynamic', self)
        solveDynamicAction.setStatusTip('Execute Dynamic analysis')

        solveMenu.addAction(buildModelAction)
        solveMenu.addAction(solveEigenAction)
        solveMenu.addAction(solveStaticAction)
        solveMenu.addAction(solveDynamicAction)

        self.show()

    def paintEvent(self, e):
        p = QPainter()
        p.begin(self)
        
        pen = QPen(Qt.black, 10)
        p.setPen(pen)
        
        size = self.size()
        w = size.width()
        h = size.height()
        
        nd = len(self.model.DOFs)
        
        if nd > 0:
            if nd > 1:
                dx = round(w*0.8/(nd-1))
            else:
                dx = round(w*0.8)
            x0 = round(w*0.1)
            y0 = round(h/2)
            i = 0
            pen = QPen(Qt.black, 20)
            p.setPen(pen)
            for id in self.model.DOFs:
                x = x0 + dx*i
                y = y0
                p.drawPoint(x, y)
                self.model.DOFs[id].loc_x = x
                self.model.DOFs[id].loc_y = y
                i += 1
                
            ns = len(self.model.Springs)
            if ns > 0:
                pen = QPen(Qt.blue, 5)
                p.setPen(pen)
                for id in self.model.Springs:
                    s = self.model.Springs[id]
                    xI = self.model.DOFs[s.dofI].loc_x
                    yI = self.model.DOFs[s.dofI].loc_y
                    xJ = self.model.DOFs[s.dofJ].loc_x
                    yJ = self.model.DOFs[s.dofJ].loc_y
                    
                    dx = (xJ - xI)
                    
                    p.drawLine(xI, yI, round(xI+dx*0.3), yJ)
                    ddx = round(dx*0.4/28)
                    ddy = 3*ddx
                    for i in range(28):
                        if i%4 == 0:
                            p.drawLine(round(xI+dx*0.3+ddx*i), yI, round(xI+dx*0.3+ddx*(i+1)), yJ+ddy)
                        elif i%4 == 1:
                            p.drawLine(round(xI+dx*0.3+ddx*i), yI+ddy, round(xI+dx*0.3+ddx*(i+1)), yJ)
                        elif i%4 == 2:
                            p.drawLine(round(xI+dx*0.3+ddx*i), yI, round(xI+dx*0.3+ddx*(i+1)), yJ-ddy)
                        elif i%4 == 3:
                            p.drawLine(round(xI+dx*0.3+ddx*i), yI-ddy, round(xI+dx*0.3+ddx*(i+1)), yJ)
                    p.drawLine(round(xI+dx*0.3+ddx*(i+1)), yI, round(xI+dx*0.7), yJ)
                    p.drawLine(round(xI+dx*0.7), yI, xJ, yJ)
        
        p.end()
        
    
    def save(self):
        fn, _ = QFileDialog.getSaveFileName(self,
                "getModelFileName","./model.rod",
                "RODS Model Files (*.rod)")
        model = pickle.dumps(self.model)
        with open(fn, 'wb') as f:
            f.write(model)
            
    def open(self):
        fn, _ = QFileDialog.getOpenFileName(self,
                "getModelFileName","./model.rod",
                "RODS Model Files (*.rod)")
        with open(fn, 'rb') as f:
            self.model = pickle.load(f)
        
        self.update()
    
    def addDOF(self):
        id = self.addDOFWidget.id.value()
        dir = self.addDOFWidget.dir.currentIndex()
        mass = self.addDOFWidget.mass.value()

        self.model.add_dof(id, dir, mass)

        self.DOFTableWidget.fillTable(self.model.DOFs)
        self.DOFTableWidget.show()

        self.fixDOFWidget.fillComboBox(self.model.DOFs.keys(),False)
        self.removeDOFWidget.fillComboBox(self.model.DOFs.keys(),False)

        self.addSpringWidget.fillComboBox(self.model.DOFs.keys(),False)
        self.update()

    def fixDOF(self):
        id = int(self.fixDOFWidget.id.currentText())

        self.model.fix_dof(id)

        self.DOFTableWidget.fillTable(self.model.DOFs)
        self.DOFTableWidget.show()

        self.removeDOFWidget.fillComboBox(self.model.DOFs.keys(),False)
    
    def freeDOF(self):
        id = int(self.fixDOFWidget.id.currentText())

        self.model.free_dof(id)

        self.DOFTableWidget.fillTable(self.model.DOFs)
        self.DOFTableWidget.show()

        self.removeDOFWidget.fillComboBox(self.model.DOFs.keys(),False)

    def removeDOF(self):
        id = int(self.removeDOFWidget.id.currentText())
        del self.model.DOFs[id]
        self.removeDOFWidget.id.removeItem(self.removeDOFWidget.id.currentIndex())

        self.DOFTableWidget.fillTable(self.model.DOFs)
        self.DOFTableWidget.show()

        self.addSpringWidget.fillComboBox(self.model.DOFs.keys(),False)
        self.update()
    
    def addSpring(self):
        id = self.addSpringWidget.id.value()
        dofI = int(self.addSpringWidget.dofI.currentText())
        dofJ = int(self.addSpringWidget.dofJ.currentText())
        k = self.addSpringWidget.parameter.value()

        self.model.add_spring(id, dofI, dofJ, k)

        self.Link1DTableWidget.fillTable(self.model.Link1Ds)
        self.Link1DTableWidget.show()
        self.update()

    def addDashpot(self):
        id = self.addDashpotWidget.id.value()
        dofI = int(self.addDashpotWidget.dofI.currentText())
        dofJ = int(self.addDashpotWidget.dofJ.currentText())
        c = self.addDashpotWidget.parameter.value()

        self.model.add_dashpot(id, dofI, dofJ, c)

        self.Link1DTableWidget.fillTable(self.model.Link1Ds)
        self.Link1DTableWidget.show()
        self.update()
    
    def buildModel(self):
        self.model.build_model()
    
    def solveEigen(self):
        self.model.solve_eigen()
        
    def addWave(self):
        id = self.addWaveWidget.id.value()
        dt = self.addWaveWidget.dt.value()
        fn = self.addWaveWidget.fn.text()
        self.model.add_wave(id, dt, fn)
        
        self.WaveTableWidget.fillTable(self.model.Waves)
        self.WaveTableWidget.show()
    
    def closeEvent(self, event):
        sys.exit(0)


if __name__ == '__main__':

    app = QApplication(sys.argv)
    w = MainWindow()

    sys.exit(app.exec_())