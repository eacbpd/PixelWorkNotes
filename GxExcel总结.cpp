SparseTableStorage
    struct DummyColumnStorage {//仿造一个ValueColumnStorage的 列存储
        uint8_t opaque[sizeof(ValueColumnStorage<size_t>)]}
            ValueColumnStorage//字面意思:表里的值按照列存储
                ColumnChunk     //字面意思:按照列存储后 其中一列的值
                    void*       _ptr;
                    size_t      _capacity;
                    size_t      _position;
                    size_t      _count;
                    ColumnChunk*_next;
                ColumnChunk*    _currentChunk;//下一列
                size_t          _size;//构造函数没有初始化它 可能是ColumnChunk个数
    std::vector<DummyColumnStorage>     _columns;
    std::vector<ColumnType>             _columnTypes//描述列的数据类型
    std::vector<const type_info*>       _columnTypeInfos;
    std::bitset<256>                    _columnBits;
    // 
    PrebuildTableData*          _prebuildData;
        ResourceHeap            _resourceHeap;      // 文本，数组heap
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
        RewindBuffer            _extraClusterHeap;  // 额外的簇信息所占用的内存（连续的
            uint8_t*            _buffer;
            uint32_t            _capacity;
            uint32_t            _position;
    //
    ExcelHeap                   _localHeap;
        GxDSA                   _dsa


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
    std::vector<ColumnType>             _columnTypes//类型数组
    uint32_t                            _resourceHeapSize//预构建对象prebuildTableData的大小
    uint32_t                            _columnCount//列数
    uint32_t                            _lineCount//行数

ClusterBuilder
    IDataClusterComposer const * const * const  _composerList//纯虚类 输入数据簇生成器 存放composerTable表中各钟Composer指针的数组
    const size_t                                _composerCount//上面元素 即Composer指针 的个数


gx::ExcelParser::parse( IExcelFileStream* stream, SparseTableStorage* sparseStorage, PrebuildTableData* prebuildTableData, TableColumnInfo const* columnInfos )
	//初始化行计数器 获得文件大小 跳过BOM文件头
	while(!headerFound)//读文件头
	if( columnInfos->size != _headerIndices.size())//宏定义与文件表头比较
        return ParseResult::HEADER_COUNT_NOT_MATCH
	for( uint32_t i = 0; i<columnInfos->size; ++i)//(宏定义里列数次)
		////再次校验列类型
	if(_headerIndices.size())//文件有列数
		_baseColumn = _headerIndices[0]//有用列的索引（有无用列
	while(!_tokenHelper.alreadyRead5Line())
		////读取5行数据 固定格式 标题<><>行后5个换行符 必须开始正文      			↓空结构体
	sparseStorage->initialize( const std::vector<ExcelHeader>& headers, PrebuildTableData* prebuildData )
		for( size_t i = 0; i<headers.size(); ++i)//拷贝到存列类型的向量
			if(headers[i].type != ColumnType::Invalid)
            	_columnTypes.push_back(headers[i].type)
		std::vector<DummyColumnStorage> _columns.resize(_columnTypes.size())//调整列数
		for( size_t colIdx = 0; colIdx<_columnTypes.size(); ++colIdx )//根据类型初始化所有列
			ColumnType type = _columnTypes[colIdx]
			switch(type)
				case ColumnType::Integer: InitColumnStorage<ExcelInt>(&_columns[colIdx]); _columnTypeInfos.push_back(&typeid(ExcelInt))
	std::vector<ExcelString> lineTokens//准备向量 存一个行数据
	for(;position<size; ++position)////文件流中剩余的数据
		if(rst==InputParseResult::Normal)
			continue
		else if (rst == InputParseResult::Finished || rst == InputParseResult::FinishedEOL)
			if (lineTokens.size()<_baseColumn)//当前行一个元素都没读到
				lineTokens.emplace_back()
			else
				if(rst == InputParseResult::Finished)//读完一个元素
					lineTokens.push_back
				else
					if(_tokenHelper.text().size())//防止一行只有一个换行也当作一行也强行添加到lineTokens里
						lineTokens.push_back
		if( rst == InputParseResult::FinishedEOL)//
			if( lineTokens.size() && lineTokens.size()> _baseColumn )
				_serializeToStorage(lineTokens, sparseStorage)// 这一行解析结束，拿结果解析
					for(uint32_t col = 0; col<_columnCount; ++col)
						auto realIdx = _headerIndices[col]
            			auto token = realIdx < tokens.size() ? tokens[realIdx] : ExcelString()
            			auto header = _headers[realIdx]
						switch(header.type)
							case ColumnType::Integer:       storage->parseValue<ExcelInt>(col, token); break


gx::CompressedTableStorage::build( SparseTableStorage* sparseStorage, ExcelHeap* heap, PrebuildTableData* prebuildData )
    CompressedTableStorage::initialize( SparseTableStorage* sparseStorage, ExcelHeap* heap, PrebuildTableData* prebuildTableData )
        // 把文本，数组的连续内存正式迁移到表格堆里
    byte dataBuffer[32*sizeof(size_t)];//32*sizeof(unsigned int)
    uint32_t lineCount = sparseStorage->lineCount();//行数
    uint32_t clusterCount = ((lineCount+31)&(~31))>>5;//簇数 32行为1簇
    auto& columnTypes = sparseStorage->columnTypes();//存储n个列类型的向量
    for(uint32_t col = 0; col<(uint32_t)columnTypes.size(); ++col)//开始遍历所有列
        switch(columnTypes[col])//判断当前列的类型
            case ColumnType::Float://列    栈缓存         簇数         稀疏存储   堆分配器 预构建的数据
                storeColumn<float>( col, dataBuffer, clusterCount, sparseStorage, heap, prebuildData)
                    _columns[col].beginBuild(heap, clusterCount)//为当前列申请内存并创建簇数个ElementCluster
                        byte* bytes = heap->allocate( (uint32_t)(sizeof(ElementCluster)*clusterCount))
                        ElementCluster* _clusters = new(bytes)ElementCluster[clusterCount]
                    for(uint32_t clusterIndex = 0; clusterIndex<clusterCount; ++clusterIndex)////遍历簇数个
                        for(uint32_t localIndex = 0; localIndex<32; ++localIndex)////每个簇的值拷贝到栈缓存dataBuffer
                            uint32_t valueIndex = (clusterIndex<<5 | localIndex)
                            if(valueIndex<lineCount)//判断是否最后一个
                                float* valuePtr = (float*)&dataBuffer[0]
                                //在dataBuffer第localIndex个元素上 存入col列第valueIndex个元素
                                valuePtr[localIndex] = sparseStorage->getValue<float>(col,valueIndex)
                                    ((ValueColumnStorage<float>*)&_columns[col])->get(valueIndex)
                                        ValueColumnStorage.ColumnChunk.read(valueIndex)
                                            ((float*)ColumnChunk._ptr)[valueIndex]
                            else
                                break            //32个值的缓存  实际存储个数 ?????
                        _columns[col].build((float*)dataBuffer, localIndex, prebuildData, &floatBuilder)//构建
                            floatBuilder->store((uint32_t*)dataBuffer, localIndex, prebuildData, output = _clusters + _clusterCount)
								//floatComposers包含Uniform Masked Bit32Table三个IDataClusterComposer(压缩类型)
                                for( size_t  j= 0; j<_composerCount; ++j)
                                    if(floatBuilder->_composerList[j]->matchAndStore32( dataBuffer, localIndex, prebuildData, output))
                                        StorageType storage//uint23 data[4] 定义缓存
                                        for( uint32_t index = 0; index<localIndex; ++index) //遍历簇里的元素
                                            storag.teable[index] = dataBuffer[index]
                                        StorageType* s = (StorageType*)prebuildData->extraClusterHeapAlloc(prototypeSize())
                                        output->dataOffset = prebuildData->clusterHeapOffset((byte*)s);
                                        output->type = (uint32_t)ClusterType::Bit32Table
                                        *s = storage //???什么东西 就被拷贝到额外的簇空间
                            ++_clusterCount
                    _columns[col].endBuild(heap, prebuildData);
                        // 处理 _extraClusterData
                        _extraClusterData = heap->allocateBytes( (uint32_t)prebuildData->extraClusterHeapSize());
                        _extraClusterDataSize = (uint32_t)prebuildData->extraClusterHeapSize();
                        memcpy(_extraClusterData, prebuildData->extraClusterHeap(), prebuildData->extraClusterHeapSize());
                        prebuildData->resetExtraClusterHeap();
    prebuildData->resetResourceHeap()




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


