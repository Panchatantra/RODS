# -*- coding: utf-8 -*-

from enum import Enum
from RODS import Direction, DynamicSolver
import RODS

import os


class Link1DType(Enum):
    Spring = 1
    Dashpot = 2
    Inerter = 3



class Motion(object):
    """docstring for Motion"""
    def __init__(self, waveId, waveScale):
        super(Motion, self).__init__()
        self.waveId = waveId
        self.waveScale = waveScale


class Model(object):
    """docstring for Model"""
    def __init__(self, zeta=0.05):
        super(Model, self).__init__()
        self.zeta = zeta
        self.DOFs = {}
        self.Link1Ds = {}
        self.Springs = {}
        self.Dashpots = {}
        self.Elements = {}
        self.Recorders = {}
        self.Material1Ds = {}
        self.Waves = {}
        self.Motions = {
            Direction.X.value: Motion(-1,1.0),
            Direction.Y.value: Motion(-1,1.0),
            Direction.Z.value: Motion(-1,1.0),
        }
        self.DynamicSolver = DynamicSolver(0)
        self.nsub = 1
        
    def add_dof(self, id, dir, mass=0.0):
        dof = DOF(id, dir, mass)
        self.DOFs[id] = dof
        
    def fix_dof(self, id):
        self.DOFs[id].isFixed = True
    
    def free_dof(self, id):
        self.DOFs[id].isFixed = False
    
    def add_spring(self, id, dofI, dofJ, k):
        spring = Spring(id, dofI, dofJ, k)
        self.Springs[id] = spring
        self.Link1Ds[id] = spring

    def add_dashpot(self, id, dofI, dofJ, c):
        dashpot = Dashpot(id, dofI, dofJ, c)
        self.Dashpots[id] = dashpot
        self.Link1Ds[id] = dashpot
    
    def add_wave(self, id, dt, fn):
        wave = Wave(id, dt, fn)
        self.Waves[id] = wave
    
    def build_model(self):
        
        RODS.clear()
        for id in self.DOFs.keys():
            dof = self.DOFs[id]
            RODS.add_dof(id, dof.dir.value, dof.mass)
            if dof.isFixed:
                RODS.fix_dof(id)
            
        for id in self.Springs.keys():
            s = self.Springs[id]
            RODS.add_spring(id, s.dofI, s.dofJ, s.k)
            
        for id in self.Dashpots.keys():
            d = self.Dashpots[id]
            RODS.add_dashpot(id, d.dofI, d.dofJ, d.c)
            
        for id in self.Waves.keys():
            w = self.Waves[id]
            RODS.add_wave(id, w.dt, w.fn.encode())
            
        RODS.assemble_matrix()
        RODS.print_info()
        
    def solve_eigen(self):
        RODS.solve_eigen()
        RODS.print_info()
    
    def active_ground_motion(self, dir, waveId, waveScale):
        self.Motions[dir] = Motion(waveId, waveScale)
    
    def set_dynamic_solver(self, s, nsub):
        self.DynamicSolver = DynamicSolver(s)
        self.nsub = nsub
    
    def solve_seismic_response(self):
        for dir in self.Motions:
            mt = self.Motions[id]
            if mt.waveId >= 0:
                RODS.active_ground_motion(dir, mt.waveId, mt.waveScale)
            
        RODS.set_dynamic_solver(self.DynamicSolver.value)
        RODS.solve_seismic_response(self.nsub)
        
    def gen_scripts(fn):
        with open(fn, 'w') as f:
            f.write("import RODS\n")


class DOF(object):
    """docstring for DOF"""
    def __init__(self, id, dir=Direction.X.value, mass=1e-9, isFixed=False):
        super(DOF, self).__init__()
        self.id = id
        self.dir = Direction(dir)
        self.mass = mass
        self.isFixed = isFixed
        self.loc_x = 0.0
        self.loc_y = 0.0

class Link1D(object):
    """docstring for Link1D"""
    def __init__(self, id, dofI, dofJ, type, *params):
        super(Link1D, self).__init__()
        self.id = id
        self.dofI = dofI
        self.dofJ = dofJ
        self.type = type
        self.params = params

class Spring(Link1D):
    """docstring for Spring"""
    def __init__(self, id, dofI, dofJ, *params):
        super(Spring, self).__init__(id, dofI, dofJ, Link1DType.Spring, *params)
        self.k = self.params[0]

class Dashpot(Link1D):
    """docstring for Dashpot"""
    def __init__(self, id, dofI, dofJ, *params):
        super(Dashpot, self).__init__(id, dofI, dofJ, Link1DType.Dashpot, *params)
        self.c = self.params[0]
        
class Recorder(object):
    """docstring for Recorder"""
    def __init__(self, id, rType, fileName, items=None):
        super(Recorder, self).__init__()
        self.id = id
        self.rType = rType
        self.fileName = fileName
        if items:
            self.items = []
        else:
            self.items = items
    
    def add_item(self, id):
        self.items.append(id)

class Material1D(object):
    """docstring for Material1D"""
    def __init__(self, id, materialType):
        super(Material1D, self).__init__()
        self.id = id
        self.materialType = materialType
        self.param = []
        
class Wave(object):
    """docstring for Wave"""
    def __init__(self, id, dt, fn):
        super(Wave, self).__init__()
        self.id = id
        self.dt = dt
        self.fn = fn
        self.name = os.path.splitext((os.path.basename(fn)))[0]
