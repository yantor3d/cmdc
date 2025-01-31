py::class_<MAngle> Angle(m, "Angle");
py::class_<MDAGDrawOverrideInfo> DAGDrawOverrideInfo(m, "DAGDrawOverrideInfo");
py::class_<MDagPath> DagPath(m, "DagPath");
py::class_<MDagPathArray> DagPathArray(m, "DagPathArray");
py::class_<MDataBlock> DataBlock(m, "DataBlock");
py::class_<MDataHandle> DataHandle(m, "DataHandle");
py::class_<MDistance> Distance(m, "Distance");
py::class_<MFn> Fn(m, "Fn");
py::class_<MFnDependencyNode> FnDependencyNode(m, "FnDependencyNode");
py::class_<MMatrix> Matrix(m, "Matrix");
py::class_<MObject> Object(m, "Object");
py::class_<MObjectArray> ObjectArray(m, "ObjectArray");
py::class_<MObjectHandle> ObjectHandle(m, "ObjectHandle");
py::class_<MPlug> Plug(m, "Plug");
py::class_<MPoint> Point(m, "Point");
py::class_<MPxData> PxData(m, "PxData");
py::class_<MQuaternion> Quaternion(m, "Quaternion");
py::class_<MSelectionList> SelectionList(m, "SelectionList");
py::class_<MStatus> Status(m, "Status");
py::class_<MString> String(m, "String");
py::class_<MTime> Time(m, "Time");
py::class_<MTypeId> TypeId(m, "TypeId");
py::class_<MVector> Vector(m, "Vector");
py::enum_<MFn::Type> fn_type(Fn, "Type");
py::class_<MUuid> Uuid(m, "Uuid");
py::class_<MNodeClass> NodeClass(m, "NodeClass");
py::class_<MDGModifier> DGModifier(m, "DGModifier");
py::class_<MFnDagNode> FnDagNode(m, "FnDagNode");
py::class_<MBoundingBox> BoundingBox(m, "BoundingBox");
py::class_<MColor> Color(m, "Color");