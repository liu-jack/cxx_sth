/***********************************************************************
* @ 批量执行函数调用，循环框架
* @ brief
	1、cServiceList/cServiceMap：间隔多久执行，不补帧，每帧最多跑一次
        *、注册的回调函数返回值即CD，0-中断，能动态指定间隔周期
        *、cServiceList按逻辑先后顺序执行，cServiceMap则是按时间顺序
        *、均是将执行过的对象挪至队尾，所以List的Run性能比Map好，但频繁增删的话，Map就优势了

    2、Service是为不同对象计时执行同一函数，定时器是计时执行各种不同的注册函数。
        *、它们设计思路一致的，构造exeTime有序表，逐帧检查执行
        *、一旦某个节点未超时，那后面的就都未超时，随即结束循环

* @ author zhoumf
* @ date 2016-11-7
************************************************************************/
#pragma once

#include <set>
#include <map>
#include <list>
#include <vector>
#include "def/MmoAssert.h"
#include "def/TypeDef.h"
typedef void ServiceObj;
typedef uint32(*RefreshFun)(ServiceObj*);

//enum ServiceDoEnum;
class iService{
public:
	virtual bool UnRegister(ServiceObj* pObj){return false;}
	virtual bool Register(ServiceObj* pObj, uint32 exeTime = 0){return false;} //哪个时刻执行
	virtual void RunSevice(uint32 time_elasped, uint32 timenow){ }//循环内的回调函数(m_func)可能调到Register、UnRegister
protected:
    typedef std::multimap<uint32, ServiceObj*> mapTimer;	//可重复键值的map（时间长度，对象）
    typedef mapTimer::iterator itMapTimer;
	typedef std::pair<uint32, ServiceObj*> mapPairOne;
	typedef std::pair<uint32, mapPairOne> mapTimePair;
	typedef std::vector<mapTimePair> mapTimePairVec;
    typedef std::pair<uint32, ServiceObj*> TimerPair;		//时间长度，对象
    typedef std::list<TimerPair> listTimer;			//list
    typedef listTimer::iterator itListTimer;
    typedef std::set<ServiceObj*>::iterator ItSet;
    typedef std::vector<ServiceObj*>::iterator ItVec;

    //ServiceDoEnum m_eService;
    bool m_bRun;
    RefreshFun m_func;
    iService(RefreshFun func) : m_bRun(false) { m_func = func; }
    virtual ~iService(){};
};

//如果后面项的时间比前面项的时间小，那么在前面项没有被pop_front前，后面项无法执行到，则会出问题
//正常使用不会出现此类情形，因为执行的同一函数，后注册的Obj一定更晚，exeTime更小
//一个service为一个类的计时器，执行单元为该类的对象，最好所有一个类的注册函数时间唯一，如果不统一建议用map
class cServiceList : public iService{
public:
	cServiceList(RefreshFun func) : iService(func){}
	listTimer m_list;
	std::set<ServiceObj*> m_setDel;

	bool Register(ServiceObj* pObj, uint32 exeTime){
		m_list.push_back(TimerPair(exeTime,pObj)); //list结构，放到最后面，在Run时调了Reg也没关系
		return true;
	}
	bool UnRegister(ServiceObj* pObj){
		for (itListTimer it = m_list.begin(); it != m_list.end(); ++it){
			if (it->second == pObj) {
                m_setDel.insert(pObj);		//添加直接加，删除则不立即删除
				return true;
			}
		}		
		return false;
	}
	void RunSevice(uint32 /*time_elasped*/, uint32 timenow){
        ItSet itSet;
		//先删除注销的计时器
        for (itListTimer it = m_list.begin(); it != m_list.end() && !m_setDel.empty(); ){
            itSet = m_setDel.find(it->second);
            if (itSet != m_setDel.end()){
                m_setDel.erase(itSet);
                it = m_list.erase(it);
            }
            else ++it;
		}
        m_setDel.clear();

		m_bRun = true;	//正在执行中，锁定
		while(!m_list.empty()) {
			TimerPair& pair = m_list.front();
			if (pair.first <= timenow)	//执行时间满足条件
			{
                if (m_setDel.find(pair.second) != m_setDel.end()){
                    m_list.pop_front();	//如果准备执行时发现已经要删除，则此次不执行，并将第一个从队列中移除
                    continue;
                }
				uint32 nextTime = m_func(pair.second);	//执行函数，并获得下一次执行时间差值
				if (nextTime > 0) Register(pair.second, timenow + nextTime);	//重新注册
				m_list.pop_front();		//删除第一个执行过的
			}
			else
				break;
		};
        m_bRun = false;
	}
};
class cServiceMap : public iService{
public:
    cServiceMap(RefreshFun func) : iService(func){}
	mapTimer m_map;
    mapTimer m_mapAdd;
	mapTimePairVec m_mapAddDecVec;	//增减记录
    std::set<ServiceObj*> m_setDel;

	bool Register(ServiceObj* pObj, uint32 exeTime){
        //MMO_ASSERT(!m_bRun);
        //m_map.insert(TimerPair(exeTime, pObj));
		m_mapAddDecVec.push_back(make_pair(0,TimerPair(exeTime, pObj)));
		return true;
	}
	bool UnRegister(ServiceObj* pObj){
		//for (itMapTimer it = m_map.begin(); it != m_map.end(); ++it){
		//	if (it->second == pObj) {
  //              m_setDel.insert(pObj);
		//		return true;
		//	}
		//}
		m_mapAddDecVec.push_back(make_pair(1,TimerPair(0, pObj)));
		return false;
	}
	void RunSevice(uint32 /*time_elasped*/, uint32 timenow){
  //      ItSet itSet;
  //      for (itMapTimer it = m_map.begin(); it != m_map.end() && !m_setDel.empty(); ){
  //          itSet = m_setDel.find(it->second);
  //          if (itSet != m_setDel.end()){
  //              m_setDel.erase(itSet);
  //              it = m_map.erase(it);
  //          }
  //          else ++it;
  //      }
		//for (itMapTimer it = m_map.begin(); it != m_map.end() && !m_setDel.empty(); ){
		//	itSet = m_setDel.find(it->second);
		//	if (itSet != m_setDel.end()){
		//		m_setDel.erase(itSet);
		//		it = m_map.erase(it);
		//	}
		//	else ++it;
		//}
  //      m_setDel.clear();

		for(mapTimePairVec::iterator it = m_mapAddDecVec.begin(); it != m_mapAddDecVec.end(); it++)
		{
			mapPairOne& mapTimerOne = it->second;
			if(it->first == 0)		//增加
			{
				m_map.insert(mapTimerOne);
			}
			else	//删除
			{
				for (itMapTimer ite = m_map.begin(); ite != m_map.end(); ){
					if (ite->second == mapTimerOne.second){
						ite = m_map.erase(ite);
					}
					else ++ite;
				}
			}
		}
		m_mapAddDecVec.clear();

		m_bRun = true;
		for (itMapTimer it = m_map.begin(); it != m_map.end(); ){
			if (it->first <= timenow)//mutilpymap所以要=号
			{
                //if (m_setDel.find(it->second) != m_setDel.end()){
                //    it = m_map.erase(it);
                //    continue;
                //}
				uint32 nextTime = m_func(it->second);
                if (nextTime > 0) m_mapAdd.insert(TimerPair(timenow + nextTime, it->second));
				it = m_map.erase(it);
			}else{
                break;
			}
		}
        m_map.insert(m_mapAdd.begin(), m_mapAdd.end());
        m_mapAdd.clear();
        m_bRun = false;
	}
};