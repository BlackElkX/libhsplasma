#include "plSceneNode.h"
#include "../plResMgr/plResManager.h"

plSceneNode::plSceneNode(PlasmaVer pv) { }
plSceneNode::~plSceneNode() {
    int i;
    for (i=0; i<SceneObjects.getSize(); i++)
        SceneObjects[i]->UnRef();
    for (i=0; i<OtherObjects.getSize(); i++)
        OtherObjects[i]->UnRef();
}

short plSceneNode::ClassIndex() { return 0x0000; }
const char* plSceneNode::ClassName() { return "plSceneNode"; }

void plSceneNode::read(hsStream* S) {
    hsKeyedObject::read(S);

    SceneObjects.setSizeNull(S->readInt());
	int i;
    for (i=0; i<SceneObjects.getSize(); i++) {
        SceneObjects[i] = plResManager::inst->readKey(S);
        SceneObjects[i]->Ref();
    }
    OtherObjects.setSize(S->readInt());
    for (i=0; i<OtherObjects.getSize(); i++) {
        OtherObjects[i] = plResManager::inst->readKey(S);
        OtherObjects[i]->Ref();
    }
}

void plSceneNode::write(hsStream* S) {
    hsKeyedObject::write(S);

    S->writeInt(SceneObjects.getSize());
	int i;
    for (i=0; i<SceneObjects.getSize(); i++)
        plResManager::inst->writeKey(S, SceneObjects[i]);
    S->writeInt(OtherObjects.getSize());
    for (i=0; i<OtherObjects.getSize(); i++)
        plResManager::inst->writeKey(S, OtherObjects[i]);
}

void plSceneNode::prcWrite(hsStream* S, pfPrcHelper* prc) {
    hsKeyedObject::prcWrite(S, prc);

    int i;
    prc->writeSimpleTag("SceneObjects");
    for (i=0; i<SceneObjects.getSize(); i++)
        SceneObjects[i]->prcWrite(S, prc);
    prc->closeTag();
    prc->writeSimpleTag("PoolObjects");
    for (i=0; i<OtherObjects.getSize(); i++)
        OtherObjects[i]->prcWrite(S, prc);
    prc->closeTag();
}
