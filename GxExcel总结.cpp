gx::ExcelHeap tableStorageHeap( 1024*1024*6 )
MBArchive* mbarch = new MBArchive()
main()
    (MBArchive::)mbarch->Initilize(const std::string & _rootPath)
        ugi::IArchive* _archive = ugi::CreateStdArchive( const std::string& _path )
            StdArchive* arch = new StdArchive()
            arch->_root = ugi::FormatFilePath(const std::string & _filepath)
            arch->_root.push_back('/')
            return arch
    excel::Excel::init(IGxArchive*_pArch, const char* mbDir, const char*binaryMbDir, int iType, bool onlyLangTxt)
        if ((Excel*)g_Excel == nullptr) 
	    	g_Excel = new Excel()
	    	tableDescriptions = new TableDescriptions()

        bool rawOnly = isEditor;
        if (!isEditor)
	    	if (const char* envSetting = getenv("useRawMB"))//stdlib头文件的api获取环境变量
	    		rawOnly = *envSetting == 't'//首字母或字符串第一位是t
	    
		auto iter = g_Excel->m_map_table_info.find("index_list")//在std::map<std::string,mb_table_info*>中找index_list
		mb_table_info* table_info = iter->second;
		if (table_info->m_table != NULL)//??? 表已经有信息,就销毁
			table_info->m_table->destroy(table_info->m_dtor)

        if (rawOnly)
            CLOG(u8"----- 仅使用原始MB表 -----");
            g_Excel->Load(_pArch, mbDir, iType, onlyLangTxt);
        else if (!g_Excel->LoadBinary(_pArch, binaryMbDir, iType, onlyLangTxt))
            g_Excel->Load(_pArch, mbDir, iType, onlyLangTxt);

    gx::excel::Excel::initialize(ExcelHeap* heap)
    gx::excel::Excel::load( char const* basePath, uint32_t loadType )


                            

ValueColumnStorage//字面意思:表里的值按照列存储
    ColumnChunk     //字面意思:按照列存储后 其中一列的值
        void*       _ptr;
        size_t      _capacity;
        size_t      _position;
        size_t      _count;
        ColumnChunk*_next;
    ColumnChunk*    _currentChunk;//下一列
    size_t          _size;//构造函数没有初始化它 可能是ColumnChunk个数


SparseTableStorage
    struct DummyColumnStorage {//仿造一个ValueColumnStorage的 列存储
        uint8_t opaque[sizeof(ValueColumnStorage<size_t>)]}
    std::vector<DummyColumnStorage>     _columns;
    std::vector<ColumnType>             _columnTypes//描述列的数据类型
    std::vector<const type_info*>       _columnTypeInfos;
    std::bitset<256>                    _columnBits;
    // 
    PrebuildTableData*                  _prebuildData;
        ResourceHeap        _resourceHeap;      // 文本，数组heap
            RewindBuffer        _rewindBuffer //解析用的临时buffer
                uint8_t*        _buffer
                uint32_t        _capacity
                uint32_t        _position
            GxDSA               _poolDSA
                tlsf_t          _tlsf
                uint32_t        _runtimeSize
                uint32_t        _usedSize
                uint32_t        _defaultPoolSize
                uint32_t        _initialPoolSize
            GxEASTLAllocator    _eastlAllocator
                const char*     _name;    // Debug name, used to track memory.
                size_t          _totalAmountOfBytesAllocated;   // keeps track of the memory currently allocated
                GxDSA*          _dsa;     // our dynamic storage allocator
            struct BufferDesc {
                byte const*     bytes;
                size_t          length;}
            eastl::set<BufferDesc, std::less<BufferDesc>, GxEASTLAllocator> _stringSet;
            eastl::set<BufferDesc, std::less<BufferDesc>, GxEASTLAllocator> _arraySet;
        RewindBuffer        _extraClusterHeap;  // 额外的簇信息所占用的内存（连续的
            uint8_t*        _buffer;
            uint32_t        _capacity;
            uint32_t        _position;
    //
    ExcelHeap                           _localHeap;
        GxDSA           _dsa


CompressedTableStorage
    byte*                               _resourceHeap //预构建对象prebuildTableData的大小 非常难懂 只看懂拷贝prebuildTableData 以后再仔细看
    CompressedColumnStorage*            _columns //列存储x列数 存储整个列的元素信息，为了取值时能快速访问，整个列所有的信息都必须是在一块连续的内存上的
        friend class CompressedTableStorage
        ElementCluster* _clusters          // one cluster = maximium 32 elements
            uint32_t type       :8         // HeapAllocType 类型
            uint32_t dataOffset :24        // 基于 _compressedData 的偏移
        byte*           _extraClusterData
        uint32_t        _clusterCount
        uint32_t        _extraClusterDataSize
    ColumnType*                         _columnTypes//类型数组
    uint32_t                            _resourceHeapSize//预构建对象prebuildTableData的大小
    uint32_t                            _columnCount//列数
    uint32_t                            _lineCount//行数


CompressedTableStorage
    byte*                               _resourceHeap;
    CompressedColumnStorage*            _columns;
    std::vector<ColumnType>             _columnTypes;
    uint32_t                            _resourceHeapSize;
    uint32_t                            _columnCount;
    uint32_t                            _lineCount;
    void CompressedTableStorage::build( SparseTableStorage* sparseStorage, ExcelHeap* heap, PrebuildTableData* prebuildData )
        bool CompressedTableStorage::initialize( SparseTableStorage* sparseStorage, ExcelHeap* heap, PrebuildTableData* prebuildTableData )
            // 把文本，数组的连续内存正式迁移到表格堆里
        byte dataBuffer[32*sizeof(size_t)];//32*sizeof(unsigned int)
        uint32_t lineCount = sparseStorage->lineCount();//行数
        uint32_t clusterCount = ((lineCount+31)&(~31))>>5;
        auto& columnTypes = sparseStorage->columnTypes();//存储每列类型的n个向量
        for(uint32_t i = 0; i<(uint32_t)columnTypes.size(); ++i)//开始遍历所有列
            switch(columnTypes[i])//判断当前列的类型
                case ColumnType::Float://列  栈缓存   跟列有关的数字_簇   稀疏存储   堆分配器 预构建的数据
                    storeColumn<float>( i, dataBuffer, clusterCount, sparseStorage, heap, prebuildData)
                        uint32_t lineCount = sparseStorage->lineCount()//行数
                        _columns[i].beginBuild(heap, clusterCount)//为当前列申请内存并创建clusterCount个ElementCluster
                        for(uint32_t clusterIndex = 0; clusterIndex<clusterCount; ++clusterIndex)//遍历簇个
                            for(uint32_t localIndex = 0; localIndex<32; ++localIndex)
                                uint32_t valueIndex = (clusterIndex<<5 | localIndex)
                                if(valueIndex<lineCount)
                                    float* valuePtr = (float*)&dataBuffer[0]
                                    valuePtr[localIndex] = sparseStorage->getValue<float>(i,valueIndex)
                                else
                                    break
                            _columns[i].build((float*)dataBuffer, i, prebuildData, &floatBuilder)
                        _columns[i].endBuild(heap, prebuildData);

