from Param cimport *

# class TransformationModelBSpline declared in TransformationModel.pxd

# keep TransformationModelInterpolated, TransformationModelLinear and
# TransformationModelBSpline in separate files. Else autowrap can not
# distinguish the getDefaultParameters() static methods


cdef extern from "<OpenMS/ANALYSIS/MAPMATCHING/TransformationModel.h>" namespace "OpenMS::TransformationModelBSpline":

    void getDefaultParameters(Param &) # wrap-attach:TransformationModelBSpline
