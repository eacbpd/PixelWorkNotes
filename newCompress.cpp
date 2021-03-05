// sub_str.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"

#include <vector>
#include <map>
#include <assert.h>

#include <fstream>
#include <string>
#include <base64.h>
#include <iostream>

// #include "bone_data_compress.h"
// 观察大量的原始子串，发现有很多是大量雷同，个别不同的，
// 所以通过将字串分解为合适的子串来排重压缩
// 7组9个参数=63byte，加上一个长度=64byte,看起来比较合适
//namespace bone_data_comp {

//const int groupsize = 7;

#pragma pack(1)
struct params_t {
	uint8_t v[9];
	params_t() {
		memset(this, 0, sizeof(params_t));
	}
};

template<int n> struct Opers {
    int mult(int x){
        return n*x;
    }
    int div(int x){
        return x / n;
    }   
    int mod(int x){
        return x % n;
    }   
};
template<> struct Opers<1> {
    int mult(int x){
        return 1;
    }
    int div(int x){
        return x;
    }   
    int mod(int x){
        return 0;
    }           
};

template<uint8_t groupsize> struct substr_t {
	uint8_t	nLength;			//	这个子串的实际参数组数
	params_t Params[groupsize];			//	n组参数
								// 为了排除重复做的比较器
	bool operator < (const substr_t& _That) const {
		if (nLength < _That.nLength) {
			return true;
		}
		else if (nLength > _That.nLength) {
			return false;
		}
		return memcmp(Params, _That.Params, nLength * sizeof(params_t)) < 0;
	}
};
//static_assert(sizeof(substr_t) == 64, "");
//static_assert(sizeof(substr_t<groupsize>) == groupsize*sizeof(params_t)+1, "");
#pragma pack()

int totalCounter = 0;
int repeatCounter = 0;


// 将整个数据串转成不相同的子串索引，以便压缩
template<uint8_t groupsize>  struct substr_pool_t {

	typedef uint16_t				index_t;

	std::vector<substr_t<groupsize>>			subStrsPool;//所有不同的子串 其中值的位置 作为map的second
	std::map<substr_t<groupsize>, index_t>		DupRemove;  //利用map的有序性 便于查找 用于去重
	
												// 去重查找子串
	index_t FindSubStr(const substr_t<groupsize>& in_SubStr) {
		++totalCounter;
		auto it = DupRemove.find(in_SubStr);
		if (it == DupRemove.end()) {
			index_t n = (index_t)subStrsPool.size();
			DupRemove[in_SubStr] = n;
			subStrsPool.push_back(in_SubStr);
			return n;
		}
		else {
			++repeatCounter;
			return 	it->second;
		}
	}

	// 最后清除下去重器，以后用不到了。
	void Cleanup() {
		DupRemove.clear();
	}

	// 将原始串转换成（可能共享的）子串序列
	void FullStrToSubStrs(const std::vector<params_t>& in_FullString, std::vector<index_t>& out_SubStrings) {
		size_t n = in_FullString.size();
		size_t k = n / groupsize;	//完整的9字符组数
		size_t r = n % groupsize;	//零头字节数 
		for (size_t i = 0; i < k; ++i) {
			substr_t<groupsize> ss;
			ss.nLength = groupsize;
			memcpy(ss.Params, in_FullString.data() + groupsize * i, groupsize * sizeof(params_t));
			out_SubStrings.push_back(FindSubStr(ss));
		}
		if (r) {
			substr_t<groupsize> ss;
			ss.nLength = (uint8_t)r;
			memcpy(ss.Params, in_FullString.data() + groupsize * k, r * sizeof(params_t));
			out_SubStrings.push_back(FindSubStr(ss));
		}
	}

	// 子串还原
	void SubStrsToFullStr(const std::vector<index_t>& in_SubStrings, std::vector<params_t>& out_FullString) const {
		out_FullString.reserve(in_SubStrings.size() * sizeof(params_t) * 9);
		for (auto i : in_SubStrings) {
			const auto& ss = subStrsPool[i];
			for (uint8_t j = 0; j < ss.nLength; ++j) {
				out_FullString.push_back(ss.Params[j]);
			}
		}
	}
};



//
//	struct BoneStorageImp {
//		std::vector<params_t> vps;
//		std::vector<substr_pool_t::index_t> vidx;
//		BoneStorageImp() {
//		}
//	};
//
//	//class BoneStorage {
//	//private:
//	//	BoneStorageImp*  _imp;
//	//public:
//	BoneStorage::BoneStorage() {
//		_imp = new BoneStorageImp;
//	}
//	/**
//	* @brief
//	*
//	* @param lineCount 指示容量（有多少行）
//	* @return true
//	* @return false
//	*/
//	bool BoneStorage::BeginBuild(uint32_t lineCount) {
//		_imp->vps.resize(128 * lineCount);
//	}
//	/**
//	* @brief
//	*
//	* @param lineIndex 行号，从0开始
//	* @param organs base64 decode出来的内容
//	* @param organCount 通过数据算出来的部位数量
//	* @param chunkIndices 临时buffer,防止重复的分配和回收
//	*/
//	void BoneStorage::BuildLine(uint32_t lineIndex, Organ* organs, uint32_t organCount, std::vector<index_t>& chunkIndices) {
//		for (int o = 0; o<organCount; ++o) {//已有数据部位索引是0~77
//			uint8_t organIndex = organs[o].id;
//			for (uint8_t i = 0; i < 9; ++i) {
//				_imp->vps[128 * lineIndex + organIndex].v[i] = organs[o].params.v[i];
//			}
//		}
//	}
//	/**
//	* @brief 结束构建
//	*
//	*/
//	void BoneStorage::EndBuild() {
//		((substr_pool_t*)_imp)->FullStrToSubStrs(vps, vidx);
//		((substr_pool_t*)_imp)->Cleanup();
//		vps.clear();
//	}
//
//	/**
//	* @brief Get the Line object
//	*
//	* @param lineIndex 要获取的行号
//	* @param organs 返回这一行所有的部位数据
//	*/
//	void BoneStorage::GetLine(uint32_t lineIndex, std::vector<Organ>& organs) const {
//		//取得该行的子串索引
//		std::vector<index_t> ageParamIdx;
//		uint8_t chunkCountPerAge = 128 / ChunkSize;
//		uint32_t cursor = chunkCountPerAge * lineIndex;
//		for (uint8_t i = 0; i < chunkCountPerAge; ++i) {
//			ageParamIdx.push_back(vidx[cursor + i]);
//		}
//		//取得128个可能器官的全部状态
//		std::vector<params_t> ageParamValues;
//		((substr_pool_t*)_imp)->SubStrsToFullStr(ageParamIdx, ageParamValues);
//		//拷贝
//		for (uint8_t o = 0; o < 128; ++o) {
//			bool hasOrgan = false;//检查has这个器官
//			for (uint8_t i = 0; i < 9; ++i) {
//				hasOrgan |= ageParamValues[o].v[i];
//			}
//			if(hasOrgan){
//				Organ tmp;
//				tmp.id = o;
//				for (uint8_t i = 0; i < 9; ++i) {
//					tmp.params.v[i] = ageParamValues[o].v[i];
//				}
//				organs.push_back(tmp);
//			}
//		}
//	}
//	/**
//	* @brief 获取总行数
//	*
//	* @return uint32_t
//	*/
//	uint32_t BoneStorage::LineCount() const {
//		uint8_t chunkCountPerAge = 128 / ChunkSize;
//		return vidx.size() / chunkCountPerAge;
//	}
//	/**
//	* @brief 获取内存占用情况
//	*
//	* @return MemoryStatistic
//	*/
//	MemoryStatistic BoneStorage::Statistic() const {
//	
//	}
//}



int main()
{
	using namespace std;
	const uint16_t organCount = 128, ageCount = 1068;
	int compressStorageTotalSize;
	ifstream ifs("boneColumn.txt");

	string base64Stage;
	vector<params_t> vps, result;
	vps.resize(organCount*ageCount);//
	for(int ageIdx=0; ageIdx<ageCount; ++ageIdx){
		getline(ifs, base64Stage);
		std::string stageStr = base64_decode(base64Stage);
		uint8_t curAgeOrgansCount = stageStr.length() / 10;
		for (int o = 0; o<curAgeOrgansCount; ++o) {//已有数据部位索引是0~77
			uint8_t organIndex = uint8_t(stageStr[o * 10]);//每10个值一个chunk 第一个值是organ的id
			for (int i = 0; i < 9; ++i){
				vps[ageIdx*organCount+organIndex].v[i] = stageStr[o * 10 + i + 1];
			}
		}
		//cout << endl;
	}
	ifs.close();

	ofstream ofs("attribute.txt");
	substr_pool_t<3> sp3;
	vector<substr_pool_t<3>::index_t> vidx3;
	sp3.FullStrToSubStrs(vps, vidx3);
	sp3.Cleanup();
	sp3.SubStrsToFullStr(vidx3, result);
	compressStorageTotalSize = sizeof(params_t)*3*sp3.subStrsPool.size()+sizeof(substr_pool_t<3>::index_t)*vidx3.size();
	ofs<<"3*9*"<<sp3.subStrsPool.size()<<"+2*"<<vidx3.size()<<"="<<compressStorageTotalSize<<endl;
	
	substr_pool_t<4> sp4;
	vector<substr_pool_t<4>::index_t> vidx4;
	sp4.FullStrToSubStrs(vps, vidx4);
	sp4.Cleanup();
	sp4.SubStrsToFullStr(vidx4, result);
	compressStorageTotalSize = sizeof(params_t)*4*sp4.subStrsPool.size()+sizeof(substr_pool_t<4>::index_t)*vidx4.size();
	ofs<<"4*9*"<<sp4.subStrsPool.size()<<"+2*"<<vidx4.size()<<"="<<compressStorageTotalSize<<endl;

	substr_pool_t<5> sp5;
	vector<substr_pool_t<5>::index_t> vidx5;
	sp5.FullStrToSubStrs(vps, vidx5);
	sp5.Cleanup();
	sp5.SubStrsToFullStr(vidx5, result);
	compressStorageTotalSize = sizeof(params_t)*5*sp5.subStrsPool.size()+sizeof(substr_pool_t<5>::index_t)*vidx5.size();
	ofs<<"5*9*"<<sp5.subStrsPool.size()<<"+2*"<<vidx5.size()<<"="<<compressStorageTotalSize<<endl;

	substr_pool_t<6> sp6;
	vector<substr_pool_t<6>::index_t> vidx6;
	sp6.FullStrToSubStrs(vps, vidx6);
	sp6.Cleanup();
	sp6.SubStrsToFullStr(vidx6, result);
	compressStorageTotalSize = sizeof(params_t)*6*sp6.subStrsPool.size()+sizeof(substr_pool_t<6>::index_t)*vidx6.size();
	ofs<<"6*9*"<<sp6.subStrsPool.size()<<"+2*"<<vidx6.size()<<"="<<compressStorageTotalSize<<endl;

	substr_pool_t<7> sp7;
	vector<substr_pool_t<7>::index_t> vidx7;
	sp7.FullStrToSubStrs(vps, vidx7);
	sp7.Cleanup();
	sp7.SubStrsToFullStr(vidx7, result);
	compressStorageTotalSize = sizeof(params_t)*7*sp7.subStrsPool.size()+sizeof(substr_pool_t<7>::index_t)*vidx7.size();
	ofs<<"7*9*"<<sp7.subStrsPool.size()<<"+2*"<<vidx7.size()<<"="<<compressStorageTotalSize<<endl;

	substr_pool_t<8> sp8;
	vector<substr_pool_t<8>::index_t> vidx8;
	sp8.FullStrToSubStrs(vps, vidx8);
	sp8.Cleanup();
	sp8.SubStrsToFullStr(vidx8, result);
	compressStorageTotalSize = sizeof(params_t)*8*sp8.subStrsPool.size()+sizeof(substr_pool_t<8>::index_t)*vidx8.size();
	ofs<<"8*9*"<<sp8.subStrsPool.size()<<"+2*"<<vidx8.size()<<"="<<compressStorageTotalSize<<endl;

	substr_pool_t<9> sp9;
	vector<substr_pool_t<9>::index_t> vidx9;
	sp9.FullStrToSubStrs(vps, vidx9);
	sp9.Cleanup();
	sp9.SubStrsToFullStr(vidx9, result);
	compressStorageTotalSize = sizeof(params_t)*9*sp9.subStrsPool.size()+sizeof(substr_pool_t<9>::index_t)*vidx9.size();
	ofs<<"9*9*"<<sp9.subStrsPool.size()<<"+2*"<<vidx9.size()<<"="<<compressStorageTotalSize<<endl;

	substr_pool_t<10> sp10;
	vector<substr_pool_t<10>::index_t> vidx10;
	sp10.FullStrToSubStrs(vps, vidx10);
	sp10.Cleanup();
	sp10.SubStrsToFullStr(vidx10, result);
	compressStorageTotalSize = sizeof(params_t)*10*sp10.subStrsPool.size()+sizeof(substr_pool_t<10>::index_t)*vidx10.size();
	ofs<<"10*9*"<<sp10.subStrsPool.size()<<"+2*"<<vidx10.size()<<"="<<compressStorageTotalSize<<endl;

	substr_pool_t<11> sp11;
	vector<substr_pool_t<11>::index_t> vidx11;
	sp11.FullStrToSubStrs(vps, vidx11);
	sp11.Cleanup();
	sp11.SubStrsToFullStr(vidx11, result);
	compressStorageTotalSize = sizeof(params_t)*11*sp11.subStrsPool.size()+sizeof(substr_pool_t<11>::index_t)*vidx11.size();
	ofs<<"11*9*"<<sp11.subStrsPool.size()<<"+2*"<<vidx11.size()<<"="<<compressStorageTotalSize<<endl;

	substr_pool_t<12> sp12;
	vector<substr_pool_t<12>::index_t> vidx12;
	sp12.FullStrToSubStrs(vps, vidx12);
	sp12.Cleanup();
	sp12.SubStrsToFullStr(vidx12, result);
	compressStorageTotalSize = sizeof(params_t)*12*sp12.subStrsPool.size()+sizeof(substr_pool_t<12>::index_t)*vidx12.size();
	ofs<<"12*9*"<<sp12.subStrsPool.size()<<"+2*"<<vidx12.size()<<"="<<compressStorageTotalSize<<endl;

	substr_pool_t<13> sp13;
	vector<substr_pool_t<13>::index_t> vidx13;
	sp13.FullStrToSubStrs(vps, vidx13);
	sp13.Cleanup();
	sp13.SubStrsToFullStr(vidx13, result);
	compressStorageTotalSize = sizeof(params_t)*13*sp13.subStrsPool.size()+sizeof(substr_pool_t<13>::index_t)*vidx13.size();
	ofs<<"13*9*"<<sp13.subStrsPool.size()<<"+2*"<<vidx13.size()<<"="<<compressStorageTotalSize<<endl;

	substr_pool_t<14> sp14;
	vector<substr_pool_t<14>::index_t> vidx14;
	sp14.FullStrToSubStrs(vps, vidx14);
	sp14.Cleanup();
	sp14.SubStrsToFullStr(vidx14, result);
	compressStorageTotalSize = sizeof(params_t)*14*sp14.subStrsPool.size()+sizeof(substr_pool_t<14>::index_t)*vidx14.size();
	ofs<<"14*9*"<<sp14.subStrsPool.size()<<"+2*"<<vidx14.size()<<"="<<compressStorageTotalSize<<endl;

	substr_pool_t<15> sp15;
	vector<substr_pool_t<15>::index_t> vidx15;
	sp15.FullStrToSubStrs(vps, vidx15);
	sp15.Cleanup();
	sp15.SubStrsToFullStr(vidx15, result);
	compressStorageTotalSize = sizeof(params_t)*15*sp15.subStrsPool.size()+sizeof(substr_pool_t<15>::index_t)*vidx15.size();
	ofs<<"15*9*"<<sp15.subStrsPool.size()<<"+2*"<<vidx15.size()<<"="<<compressStorageTotalSize<<endl;

	substr_pool_t<16> sp16;
	vector<substr_pool_t<16>::index_t> vidx16;
	sp16.FullStrToSubStrs(vps, vidx16);
	sp16.Cleanup();
	sp16.SubStrsToFullStr(vidx16, result);
	compressStorageTotalSize = sizeof(params_t)*16*sp16.subStrsPool.size()+sizeof(substr_pool_t<16>::index_t)*vidx16.size();
	ofs<<"16*9*"<<sp16.subStrsPool.size()<<"+2*"<<vidx16.size()<<"="<<compressStorageTotalSize<<endl;

	substr_pool_t<17> sp17;
	vector<substr_pool_t<17>::index_t> vidx17;
	sp17.FullStrToSubStrs(vps, vidx17);
	sp17.Cleanup();
	sp17.SubStrsToFullStr(vidx17, result);
	compressStorageTotalSize = sizeof(params_t)*17*sp17.subStrsPool.size()+sizeof(substr_pool_t<17>::index_t)*vidx17.size();
	ofs<<"17*9*"<<sp17.subStrsPool.size()<<"+2*"<<vidx17.size()<<"="<<compressStorageTotalSize<<endl;

	substr_pool_t<18> sp18;
	vector<substr_pool_t<18>::index_t> vidx18;
	sp18.FullStrToSubStrs(vps, vidx18);
	sp18.Cleanup();
	sp18.SubStrsToFullStr(vidx18, result);
	compressStorageTotalSize = sizeof(params_t)*18*sp18.subStrsPool.size()+sizeof(substr_pool_t<18>::index_t)*vidx18.size();
	ofs<<"18*9*"<<sp18.subStrsPool.size()<<"+2*"<<vidx18.size()<<"="<<compressStorageTotalSize<<endl;

	ofs.close();
	//bool err = false;
	//for (int i = 0; i < vps.size(); ++i){
	//	if (memcmp(&vps[i], &result[i], 9)) {
	//		err = true;
	//	}
	//}
	//uint16_t monitor[65536] = {};
	//for (auto a : vidx)
	//	++monitor[a];
	//
	//getchar();
	return 0;
}

/*可能最佳组大小是16, 结果如下:// FIXME

组长*去重后条数	 +		索引长*索引数 =总占用字节
2*9*(9305)		+		2(68352)	=304194
3*9*(12831)		+		2(45568)	=437573
4*9*(6406)		+		2(34176)	=298968
5*9*(11022)		+		2(27341)	=550672
6*9*(8227)		+		2(22784)	=489826
7*9*(9199)		+		2(19530)	=618597
8*9*(4049)		+		2(17088)	=325704
9*9*(8112)		+		2(15190)	=687452

10*9*(6668)		+		2(13671)	=627462
11*9*(6997)		+		2(12428)	=717559
12*9*(5009)		+		2(11392)	=563756
13*9*(6270)		+		2(10516)	=754622
14*9*(5470)		+		2(9765)		=708750
15*9*(5597)		+		2(9114)		=773823
16*9*(2579)		+		2(8544)		=388464
17*9*(5119)		+		2(8042)		=799291
18*9*(4800)		+		2(7595)		=792790
19*9*(4740)		+		2(7195)		=824930

20*9*(3992)		+		2(6836)		=732232
*/


















