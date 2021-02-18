void Graphics::setupTerra() {
 
     registerHlms< Ogre::HlmsTerra>();

    auto appendToPbs = [&]() {

        Ogre::HlmsPbs* hlmsPbs = dynamic_cast<Ogre::HlmsPbs*>(mRoot->getHlmsManager()->getHlms(Ogre::HLMS_PBS));
        Ogre::Archive* archivePbs = hlmsPbs->getDataFolder();
        Ogre::ArchiveVec libraryPbs = hlmsPbs->getPiecesLibraryAsArchiveVec();
        libraryPbs.push_back(Ogre::ArchiveManager::getSingletonPtr()->load(
            mHlmsPath + "Hlms/Terra/" + mShaderSyntax + "/PbsTerraShadows",
            "FileSystem", true));
        hlmsPbs->reloadFrom(archivePbs, &libraryPbs);
    };
    appendToPbs();

    Ogre::String resourcesPath = mHlmsPath + "Terra/";
    addResourceLocations({ resourcesPath, resourcesPath + "HLSL", resourcesPath + "GLSL",  resourcesPath + "METAL" }, "FileSystem", "Terra" );



    setupCompositorDefault("Tutorial_TerrainWorkspace");
}

			auto createTerra = [&]()->auto{

				// Render terrain after most objects, to improve performance by taking advantage of early Z
				Ogre::Terra* terra = new Ogre::Terra(Ogre::Id::generateNewId<Ogre::MovableObject>(),
					&sceneManager->_getEntityMemoryManager(Ogre::SCENE_STATIC),
					sceneManager, 11u, root->getCompositorManager2(),
					graphics.getCamera());

				terra->setCastShadows(false);

				terra->load( heightMapFilename, origin, dimensions);

				Ogre::SceneNode* rootNode = sceneManager->getRootSceneNode(Ogre::SCENE_STATIC);
				Ogre::SceneNode* sceneNode = rootNode->createChildSceneNode(Ogre::SCENE_STATIC);
				sceneNode->attachObject(terra);

				Ogre::HlmsManager* hlmsManager = root->getHlmsManager();
				Ogre::HlmsDatablock* datablock = hlmsManager->getDatablock(materialName);//"TerraExampleMaterial"
				//        Ogre::HlmsDatablock *datablock = hlmsManager->getHlms( Ogre::HLMS_USER3 )->getDefaultDatablock();
				//        Ogre::HlmsMacroblock macroblock;
				//        macroblock.mPolygonMode = Ogre::PM_WIREFRAME;
						//datablock->setMacroblock( macroblock );
				terra->setDatablock(datablock);


				Ogre::HlmsPbsTerraShadows* shadowListener = new Ogre::HlmsPbsTerraShadows();
				shadowListener->setTerra(terra);
				//Set the PBS listener so regular objects also receive terrain shadows
				Ogre::Hlms* hlmsPbs = root->getHlmsManager()->getHlms(Ogre::HLMS_PBS);
				hlmsPbs->setListener(shadowListener);

				return std::make_tuple(terra, sceneNode, shadowListener);
			};