#define _doc_create R"pbdoc(Creates a new DAG node of the specified type, with the given name.\
The type may be either a type name or a type ID. If no name is given\
then a unique name will be generated by combining the type name with\
an integer.\
\
If a parent is given then the new node will be parented under it and\
the functionset will be attached to the newly-created node. The\
newly-created node will be returned.\
\
If no parent is given and the new node is a transform, it will be\
parented under the world and the functionset will be attached to the\
newly-created transform. The newly-created transform will be returned.\
\
If no parent is given and the new node is not a transform then a\
transform node will be created under the world, the new node will be\
parented under it, and the functionset will be attached to the\
transform. The transform will be returned.)pbdoc"

#define _doc_addChild R"pbdoc(Makes this node a parent of `child`.)pbdoc"

#define _doc_child R"pbdoc(Returns the specified child of this node.)pbdoc"

#define _doc_childCount R"pbdoc(Returns the number of nodes which are children of this one.)pbdoc"

#define _doc_getConnectedSetsAndMembers R"pbdoc( Returns a tuple containing an array of sets and an array of the\
components of the DAG object which are in those sets. If the entire object is in a set, then the corresponding entry in the comps array will have no elements in it.\
)pbdoc"

#define _doc_dagPath R"pbdoc(Returns the DAG path to which this function set is attached. Raises a TypeError if the function set is attached to an MObject rather than a path.)pbdoc"

#define _doc_dagRoot R"pbdoc(Returns the root node of the first path leading to this node.)pbdoc"

#define _doc_duplicate R"pbdoc(Duplicates the DAG hierarchy rooted at the current node.)pbdoc"

#define _doc_fullPathName R"pbdoc(Returns the full path of the attached object, from the root of the DAG on down.)pbdoc"

#define _doc_getAllPaths R"pbdoc(Returns all of the DAG paths which lead to the object to which this function set is attached.)pbdoc"

#define _doc_getPath R"pbdoc(Returns the DAG path to which this function set is attached, or the first path to the node if the function set is attached to an MObject.)pbdoc"

#define _doc_hasChild R"pbdoc(Returns True if the specified node is a child of this one.)pbdoc"

#define _doc_hasParent R"pbdoc(Returns True if the specified node is a parent of this one.)pbdoc"

#define _doc_instanceCount R"pbdoc(Returns the number of instances for this node.)pbdoc"

#define _doc_isChildOf R"pbdoc(Returns True if the specified node is a parent of this one.)pbdoc"

#define _doc_isInstanced R"pbdoc(Returns True if this node is instanced.)pbdoc"

#define _doc_isInstancedAttribute R"pbdoc(Returns True if the specified attribute is an instanced attribute of this node.)pbdoc"

#define _doc_isParentOf R"pbdoc(Returns True if the specified node is a child of this one.)pbdoc"

#define _doc_parent R"pbdoc(Returns the specified parent of this node.)pbdoc"

#define _doc_parentCount R"pbdoc(Returns the number of parents this node has.)pbdoc"

#define _doc_partialPathName R"pbdoc(Returns the minimum path string necessary to uniquely identify the attached object.)pbdoc"

#define _doc_removeChild R"pbdoc(Removes the child, specified by MObject, reparenting it under the world.)pbdoc"

#define _doc_removeChildAt R"pbdoc(Removes the child, specified by index, reparenting it under the world.)pbdoc"

#define _doc_setObject R"pbdoc(Attaches the function set to the specified node or DAG path.)pbdoc"

#define _doc_setObject R"pbdoc(Attaches the function set to the specified node or DAG path.)pbdoc"

#define _doc_transformationMatrix R"pbdoc(Returns the object space transformation matrix for this DAG node.)pbdoc"

py::enum_<MFnDagNode::MObjectColorType>(FnDagNode, "ObjectColorType")
    .value("kUseDefaultColor", MFnDagNode::MObjectColorType::kUseDefaultColor) 
    .value("kUseIndexColor", MFnDagNode::MObjectColorType::kUseIndexColor) 
    .value("kUseRGBColor", MFnDagNode::MObjectColorType::kUseRGBColor) 
    .export_values();

FnDagNode
    .def_property_readonly_static("kNextPos", [](py::object /* self */) {
        return static_cast<unsigned int>(MFnDagNode::kNextPos); 
    })

    .def(py::init<>())

    .def(py::init([](MObject& object) {
        MStatus status;
        MFnDagNode fn { object, &status };

        if (!status) {
            throw std::runtime_error(
                "Invalid parameter passed for object - "
                "not a DAG Node, "
                "Node does not exist or "
                "no valid pointer to Node"
            );
        }

        return std::unique_ptr<MFnDagNode>(new MFnDagNode(object));
    }))

    .def(py::init([](MDagPath& dagPath) {
        MStatus status;
        MFnDagNode fn { dagPath, &status };

        if (!status) {
            throw std::runtime_error(
                "Invalid parameter passed for object - "
                "not a DAG Node, "
                "Node does not exist or "
                "no valid pointer to Node"
            );
        }

        return std::unique_ptr<MFnDagNode>(new MFnDagNode(dagPath));
    }))

    .def("__repr__", [](const MObject &a) {
        return "<cmdc.FnDagNode()>";
    })

    .def("addChild", [](MFnDagNode& self,
                        MObject& child,
                        unsigned int index = static_cast<unsigned int>(MFnDagNode::kNextPos),
                        bool keepExistingParents = false) {
        MStatus status = self.addChild(child, index, keepExistingParents);

        if (!status) {
            /**
             * There are three know possible reasons this might not work.
             * 
             */

            // 1. Not a valid pointer
            if (child.isNull()) {
                throw std::runtime_error("MObject was null.");
            }

            // 2. Not a DAG node
            if (!child.hasFn(MFn::kDagNode)) {
                MFnDagNode fn { child };
                MString message = fn.partialPathName();
                message += " was not a DAG node.";
                throw std::runtime_error(message.asChar());
            }

            // 3. Node does not exist
            MObjectHandle handle { child };
            if (!(handle.isValid() && handle.isAlive())) {
                throw std::runtime_error("MObject did not exist");
            }

            // 4. The docs don't say why this happens
            throw std::runtime_error("Undefined error occurred");
        }

    }, py::arg("child"),
       py::arg("index") = static_cast<unsigned int>(MFnDagNode::kNextPos),
       py::arg("keepExistingParents") = false,
       _doc_addChild)

    .def("boundingBox", [](MFnDagNode& self) -> MBoundingBox {
        return self.boundingBox();
    }, R"pbdoc(Node's bounding box, in object space.)pbdoc")

    .def("child", [](MFnDagNode& self, unsigned int i) -> MObject {
        return self.child(i);
    }, _doc_child)

    .def("childCount", [](MFnDagNode& self) -> int {
        return self.childCount();
    }, _doc_childCount)

    .def("create", [](MFnDagNode& self, std::string type, MObject parent = MObject::kNullObj) -> MObject {
        return self.create(type.c_str(), parent);
    }, py::arg("type"),
       py::arg("parent") = MObject::kNullObj,
       _doc_create)

    .def("create", [](MFnDagNode& self, std::string type, std::string name, MObject parent = MObject::kNullObj) -> MObject {
        return self.create(type.c_str(), name.c_str(), parent);
    }, py::arg("type"),
       py::arg("name"),
       py::arg("parent") = MObject::kNullObj,
       _doc_create)

    .def("create", [](MFnDagNode& self, MTypeId typeId, MObject parent = MObject::kNullObj) -> MObject {
        return self.create(typeId, parent);
    }, py::arg("type"),
       py::arg("parent") = MObject::kNullObj,
       _doc_create)

    .def("create", [](MFnDagNode& self, MTypeId typeId, std::string name, MObject parent = MObject::kNullObj) -> MObject {
        return self.create(typeId, name.c_str(), parent);
    }, py::arg("type"),
       py::arg("name"),
       py::arg("parent") = MObject::kNullObj,
       _doc_create)

    .def("dagPath", [](MFnDagNode& self) -> MDagPath {
        return self.dagPath();
    }, _doc_dagPath)

    .def("dagRoot", [](MFnDagNode& self) -> MObject {
        return self.dagRoot();
    }, _doc_dagRoot)

    .def("duplicate", [](MFnDagNode& self, bool instance = false, bool instanceLeaf = false) -> MObject {
        return self.duplicate(instance, instanceLeaf);
    }, py::arg("instance") = false,
       py::arg("instanceLeaf") = false,
       _doc_duplicate)

    .def("fullPathName", [](MFnDagNode& self) -> std::string {
        return self.fullPathName().asChar();
    }, _doc_fullPathName)

    .def("getAllPaths", [](MFnDagNode& self) -> MDagPathArray {




        // Need MDagPathArray
        throw std::logic_error{"Function not yet implemented."};




    }, _doc_getAllPaths)

    .def("getConnectedSetsAndMembers", [](MFnDagNode& self, unsigned int instanceNumber, bool renderableSetsOnly) -> std::tuple<MObjectArray, MObjectArray> {





        // Need std::tuple<MObjectArray, MObjectArray>
        throw std::logic_error{"Function not yet implemented."};






    }, _doc_getConnectedSetsAndMembers)

    .def("getPath", [](MFnDagNode& self) -> MDagPath {
        MDagPath path;
        const MStatus status = self.getPath(path);

        if (!status) {
            throw std::runtime_error(
                "No valid DAG Node attached to Function Set"
            );
        }

        return path;
    }, _doc_getPath)

    .def("hasChild", [](MFnDagNode& self, MObject node) -> bool {
        return self.hasChild(node);
    }, py::arg("node"), _doc_hasChild)

    .def("hasParent", [](MFnDagNode& self, MObject node) -> bool {
        return self.hasParent(node);
    }, py::arg("node"), _doc_hasParent)

    .def("inModel", [](MFnDagNode& self) -> bool {
        return self.inModel();
    }, R"pbdoc(True if the node has been added to the model.)pbdoc")

    .def("inUnderWorld", [](MFnDagNode& self) -> bool {
        return self.inUnderWorld();
    }, R"pbdoc(True if this node is in the underworld of another node (e.g. a curve on surface is in the underworld of the surface).)pbdoc")

    .def("instanceCount", [](MFnDagNode& self, bool total) -> int {
        return self.instanceCount(total);
    }, py::arg("total"), _doc_instanceCount)

    .def("isChildOf", [](MFnDagNode& self, MObject node) -> bool {
        return self.isChildOf(node);
    }, py::arg("node"), _doc_isChildOf)

    .def("isInstanceable", [](MFnDagNode& self) -> bool {
        return self.isInstanceable();
    }, R"pbdoc(True if instancing is allowed for this node.)pbdoc")

    .def("isInstanced", [](MFnDagNode& self, bool indirect = true) -> bool {
        return self.isInstanced(indirect);
    }, py::arg("indirect"), _doc_isInstanced)

    .def("isInstancedAttribute", [](MFnDagNode& self, MObject attr) -> bool {
        return self.isInstancedAttribute(attr);
    }, py::arg("attr"), _doc_isInstancedAttribute)

    .def("isIntermediateObject", [](MFnDagNode& self) -> bool {
        return self.isIntermediateObject();
    }, R"pbdoc(True if this node is just an intermediate in part of a larger calculation (e.g. input to a deformer).)pbdoc")

    .def("isParentOf", [](MFnDagNode& self, MObject node) -> bool {
        return self.isParentOf(node);
    }, py::arg("node"), _doc_isParentOf)

    .def("objectColorRGB", [](MFnDagNode& self) -> MColor {
        return self.objectColorRGB();
    }, R"pbdoc(RGB value indicating the color in which the node is to be drawn when inactive, assuming that it is drawable.)pbdoc")

    .def("objectColorType", [](MFnDagNode& self) -> MFnDagNode::MObjectColorType {
        return self.objectColorType();
    }, R"pbdoc(Determines whether the default color, indexed object color, orRGB object color is used for this object.)pbdoc")

    .def("parent", [](MFnDagNode& self, unsigned int i) -> MObject {
        return self.parent(i);
    }, _doc_parent)

    .def("parentCount", [](MFnDagNode& self) -> int {
        return self.parentCount();
    }, _doc_parentCount)

    .def("partialPathName", [](MFnDagNode& self) -> std::string {
        return self.partialPathName().asChar();
    }, _doc_partialPathName)

    .def("removeChild", [](MFnDagNode& self, MObject& child) -> void {
        MStatus status = self.removeChild(child);

        if (status == MStatus::kSuccess) { return; }

        else if (status == MStatus::kInvalidParameter) {
            throw std::runtime_error(
                "Invalid parameter passed as child - not a DAG Node, Node does not exist or no valid pointer to Nod"
            );
        }

        else if (status == MStatus::kFailure) {
            throw std::runtime_error(
                "No valid DAG Node attached to Function Set"
            );
        }

        else {
            throw std::runtime_error(
                "Unknown error occurred."
            );
        }

    }, _doc_removeChild)

    .def("removeChildAt", [](MFnDagNode& self, unsigned int index) {
        return self.removeChildAt(index);
    }, py::arg("index"), _doc_removeChildAt)

    .def("setObject", [](MFnDagNode& self, MObject& object) -> void {
        if (!self.setObject(object)) throw std::runtime_error(
            "Invalid parameter passed for node - "
            "not a DAG Node, "
            "Node does not exist or "
            "no valid pointer to Node"
        );
    }, _doc_setObject)

    .def("setObject", [](MFnDagNode& self, MDagPath path) {
        if (!self.setObject(path)) throw std::runtime_error(
            "Invalid parameter passed for objectPath - "
            "invalid Path or "
            "target Node is not a DAG Node, "
            "does not exist or "
            "has no accessible pointer "
        );
    }, py::arg("path"), _doc_setObject)

    .def("transformationMatrix", [](MFnDagNode& self) -> MMatrix {
        return self.transformationMatrix();
    }, _doc_transformationMatrix);