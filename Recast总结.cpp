RecastDemo https://zhuanlan.zhihu.com/p/74537236
SDL_Init(unsigned int flags)//创建窗口\管线\GL上下文,设置窗口坐标
imguiRenderGLInit("DroidSans.ttf")//初始化字体文件
SDL_PollEvent(unsigned int event)//输入事件处理
    Sample_SoloMesh::SampleTool::handleToggle()//PATHFIND_FOLLOW单步寻路
    InputGeom::saveGeomSet(const BuildSettings* settings)//保存设置
InputGeom::raycastMesh(float* src, float* dst, float& tmin)//光线投射网格,计算鼠标点击的体素
    isectSegAABB(const float* sp, const float* sq, const float* amin, const float* amax, float& tmin, float& tmax)
    rcGetChunksOverlappingSegment(const rcChunkyTriMesh* cm, float p[2], float q[2], int* ids, const int maxIds)
        checkOverlapSegment(const float p[2], const float q[2], const float bmin[2], const float bmax[2])
    intersectSegmentTriangle(const float* sp, const float* sq, const float* a, const float* b, const float* c, float &t)
virtual Sample::handleClick(const float* s, const float* p, bool shift)//处理点击事件
    NavMeshTesterTool::handleClick()//以下仅当m_tool为NavMeshTesterTool的invoke
        NavMeshTesterTool::recalc()
            Sample_SoloMesh::NavMeshTesterTool::dtNavMeshQuery::findNearestPoly()
            if (m_toolMode == TOOLMODE_PATHFIND_FOLLOW)//跟踪寻路模式
                Sample_SoloMesh::NavMeshTesterTool::dtNavMeshQuery::findPath()
                Sample_SoloMesh::NavMeshTesterTool::dtNavMeshQuery::closestPointOnPoly()
                while (npolys && m_nsmoothPath < MAX_SMOOTH)
                    Sample_SoloMesh::NavMeshTesterTool::getSteerTarget()
                    Sample_SoloMesh::NavMeshTesterTool::dtNavMeshQuery::moveAlongSurface()
                    fixupCorridor()
                    fixupShortcuts()
                    Sample_SoloMesh::NavMeshTesterTool::dtNavMeshQuery::getPolyHeight()
                    if (endOfPath && inRange(iterPos, steerPos, SLOP, 1.0f))
                    else if (offMeshConnection && inRange(iterPos, steerPos, SLOP, 1.0f))
                        dtNavMesh::getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos, endPos)
                        Sample_SoloMesh::NavMeshTesterTool::dtNavMeshQuery::getPolyHeight(polys[0], iterPos, &eh)


virtual Sample::handleUpdate(const float dt)//更新模型信息 20fps
    NavMeshTesterTool::handleUpdate(const float /*dt*/)//以下仅当m_tool为NavMeshTesterTool更新的数据
        if (m_toolMode == TOOLMODE_PATHFIND_SLICED)
            if (dtStatusInProgress(m_pathFindStatus))
                dtNavMeshQuery::updateSlicedFindPath(const int maxIter, int* doneIters)
            if (dtStatusSucceed(m_pathFindStatus))
                dtNavMeshQuery::finalizeSlicedFindPath(dtPolyRef* path, int* pathCount, const int maxPath)
                if (m_npolys)
                    dtNavMeshQuery::closestPointOnPoly(dtPolyRef ref, const float* pos, float* closest, bool* posOverPoly) const
                    dtNavMeshQuery::findStraightPath(const float* startPos, const float* endPos, const dtPolyRef* path,
                     const int pathSize,float* straightPath, unsigned char* straightPathFlags, dtPolyRef* straightPathRefs,
                     int* straightPathCount, const int maxStraightPath, const int options) const                  
    updateToolStates(dt)//m_toolStates[i]->handleUpdate(dt)遍历更新所有状态
        CrowdToolState::handleUpdate(const float dt)//Crowd 集群寻路调用的函数
        MonsterToolState::handleUpdate(const float dt)//自行实现的cs寻路
dtTileCache::update(const float /*dt*/, dtNavMesh* navmesh, bool* upToDate)//子类Sample_TempObstacles额外处理了导航网格


Sample_SoloMesh::handleRender()//渲染模型信息
    //一般地形渲染
    SampleTool::handleRender()//工具内容的渲染
    Sample::renderToolStates()//状态渲染
        m_toolStates[i]->handleRender()//CrowdToolState::handleRender()//项目源码只实现了CrowdToolState 可以手动扩展一些
TestCase::handleRender()
Sample_SoloMesh::handleRenderOverlay(double* proj, double* model, int* view)
    Sample_SoloMesh::NavMeshTesterTool::handleRenderOverlay(double* proj, double* model, int* view)
    Sample_SoloMesh::renderOverlayToolStates(double* proj, double* model, int* view)
        CrowdToolState::handleRenderOverlay(double* proj, double* model, int* view)
TestCase::handleRenderOverlay(double* proj, double* model, int* view)


////other settings////
if (showMenu)
    ///设置一些对话框的触发条件///
    if(geom && sample)//采样和几何模型已具备
        virtual Sample::handleSettings()//右侧菜单工具栏设置
        virtual Sample::handleBuild()//生成网格 以下仅当Sample实例为Sample_SoloMesh时
            // Step 1. Initialize build config.填写build的配置结构体rcConfig
            Sample_SoloMesh::rcConfig m_cfg
            // Step 2. Rasterize input polygon soup.将原网格光栅化（Rasterization）,或称体素化（Voxelization）
            rcCreateHeightfield(rcContext* ctx, rcHeightfield& hf, int width, int height,
        		const float* bmin, const float* bmax, float cs, float ch)
            rcMarkWalkableTriangles(rcContext* ctx, const float walkableSlopeAngle, const float* verts,
                int nv, const int* tris, int nt, unsigned char* areas)
            rcRasterizeTriangles(rcContext* ctx, const float* verts, const int /*nv*/, const int* tris, 
                const unsigned char* areas, const int nt, rcHeightfield& solid, const int flagMergeThr)
                rasterizeTri(const float* v0, const float* v1, const float* v2, const unsigned char area, rcHeightfield& hf,
        			const float* bmin, const float* bmax, const float cs, const float ics, const float ich, const int flagMergeThr)
            // Step 3. Filter walkables surfaces.过滤可行走表面(台阶 悬崖 隧道)
            rcFilterLowHangingWalkableObstacles(rcContext* ctx, const int walkableClimb, rcHeightfield& solid)
            rcFilterLedgeSpans(rcContext* ctx, const int walkableHeight, const int walkableClimb, rcHeightfield& solid)
            rcFilterWalkableLowHeightSpans(rcContext* ctx, int walkableHeight, rcHeightfield& solid)
            // Step 4. Partition walkable surface to simple regions.
            //根据计算出来的行走面，使用特定算法，将这些可行走面切分为一个个尽量大的、连续的、不重叠的、中间没有“洞”的“区域”(Region)
            //Recast 提供了三种算法 Watershed Monotone Layer 压缩三维空间的高,生成二维空间,保证不重叠
            rcBuildCompactHeightfield(rcContext* ctx, const int walkableHeight, const int walkableClimb,
                rcHeightfield& hf, rcCompactHeightfield& chf)
            rcErodeWalkableArea(rcContext* ctx, int radius, rcCompactHeightfield& chf)//过滤小块
            if (m_partitionType == SAMPLE_PARTITION_WATERSHED)
                rcBuildDistanceField(rcContext* ctx, rcCompactHeightfield& chf)
                rcBuildRegions(rcContext* ctx, rcCompactHeightfield& chf,
        			const int borderSize, const int minRegionArea, const int mergeRegionArea)
            // Step 5. Trace and simplify region contours.根据体素化信息和 Region，首先构建出描绘 Region 的 Detailed Contours（精确轮廓）。
            //由于 Detailed Contour 以体素为单位构建边缘的，因此是锯齿状的。接着，再将 Detailed Contours 简化为 Simplified Contours（简化轮廓），
            //方便后面的做三角形化（Triangulation）。在这一步之后，体素化数据就不再会被使用了。
            rcBuildContours(rcContext* ctx, rcCompactHeightfield& chf, const float maxError, const int maxEdgeLen,
        		rcContourSet& cset, const int buildFlags)
            // Step 6. Build polygons mesh from contours.
            //大多数算法处理需要基于凸多边形，这一步就是将 Simplified Contours 切分为多个凸多边形
            //在一个Polygon中,任意两个点在二维平面内都是可以直线到达的。因此，Polygon 是 Detour 的基本寻路单元。
            rcBuildPolyMesh(rcContext* ctx, rcContourSet& cset, const int nvp, rcPolyMesh& mesh)
            // Step 7. Create detail mesh which allows to access approximate height on each polygon.
            rcBuildPolyMeshDetail(rcContext* ctx, const rcPolyMesh& mesh, const rcCompactHeightfield& chf, const float sampleDist, 
                const float sampleMaxError, rcPolyMeshDetail& dmesh)
    virtual Sample::handleDebugMode()//图形化显示导航的调试信息

if (showSample)//采样模式选择对话框
    sample=g_samples[i].create()
    Sample_SoloMesh::setContext(BuildContext* ctx)
    if (newSample)
        if (geom)
            Sample_SoloMesh::handleMeshChanged(InputGeom* geom)//如果加载的是之前save的.gset文件,使用其已设定的参数
if (showLevels)//水平面选择对话框
    geom = new InputGeom
    InputGeom::load(rcContext* ctx, const std::string& filepath)//从.obj文件中获取顶点信息,创建三角形
        rcCalcBounds(const float* verts, int nv, float* bmin, float* bmax)//计算模型坐标范围
        rcCreateChunkyTriMesh(const float* verts, const int* tris, int ntris, int trisPerChunk, rcChunkyTriMesh* cm)//简单处理下三角形
    if (sample && geom)
        Sample_SoloMesh::handleMeshChanged(InputGeom* geom)
if (showTestCases)
    if (testToLoad != filesEnd)
        if (test)
            TestCase::load(const std::string& filePath)
            g_samples[i].create()
            if (sample)
                Sample_SoloMesh::setContext(BuildContext* ctx)
            geom = new InputGeom
            if (sample && geom)
                Sample_SoloMesh::handleMeshChanged(InputGeom* geom)
            if (sample)
                Sample_SoloMesh::handleSettings()
            Sample_SoloMesh::handleBuild()
            if (sample)
                TestCase::doTests(dtNavMesh* navmesh, dtNavMeshQuery* navquery)
////show logs////
if (!showTestCases && showTools && showMenu)//Left column tools menu
    if (sample)
        Sample_SoloMesh::handleTools()//左侧菜单加载当前采样可用的工具
            m_tool->handleMenu()//处理工具的菜单状态
////Maker////
imguiRenderGLDraw()