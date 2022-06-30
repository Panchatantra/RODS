import numpy as np
import pandas as pd

def read_frame_from_sap2000(fn):
    df_JC = pd.read_excel(fn, sheet_name="Joint Coordinates",
                          header=[1,2],
                          engine="openpyxl")
    df_JC = df_JC.sort_values(("Joint", "Text"))
    nodeCoord = df_JC[["Joint","XorR","Y","Z"]].values
    
    df_JM = pd.read_excel(fn, sheet_name="Joint Added Mass Assignments",
                          header=[1,2],
                          engine="openpyxl")
    df_JM = df_JM.sort_values(("Joint", "Text"))
    nodeMass = df_JM[["Mass1"]].values
    nodeInfo = np.hstack((nodeCoord, nodeMass)).tolist()
    for i in range(len(nodeInfo)):
        nodeInfo[i][0] = int(nodeInfo[i][0])
    
    df_JR = pd.read_excel(fn, sheet_name="Joint Restraint Assignments",
                          header=[1,2],
                          engine="openpyxl")
    df_JR = df_JR.sort_values(("Joint", "Text"))
    fixedNodes = df_JR[["Joint"]].values.T[0]
    
    df_FC = pd.read_excel(fn, sheet_name="Connectivity - Frame",
                          header=[1,2],
                          engine="openpyxl")
    df_FC = df_FC.sort_values(("Frame", "Text"))
    frameConnect = df_FC[["Frame","JointI","JointJ"]].values
    
    df_FS = pd.read_excel(fn, sheet_name="Frame Section Assignments",
                          header=[1,2],
                          engine="openpyxl")
    df_FS = df_FS.sort_values(("Frame", "Text"))
    frameSection = df_FS[["AnalSect"]].values
    
    frameInfo = np.hstack((frameConnect, frameSection))
    
    return nodeInfo, frameInfo, fixedNodes


if __name__ == "__main__":
    nodeInfo, frameInfo, fixedNodes = read_frame_from_sap2000("frame3d.xlsx")
    # print(nodeInfo)
    # print(frameInfo.dtype)
