#pragma once


class C101PPTService
{
public:
	C101PPTService(void);
	~C101PPTService(void);
public:
	BOOL CreateSvc();         ///<����Service
	BOOL DeleteSvc();         ///<ɾ��Service
	BOOL StartSvc();          ///<����Service
	BOOL StopSvc();           ///<ֹͣService
private:
	void CloseSvcHandles();      ///<������
	/** \brief ֹͣService
	*
	* @param hSCM - Handle to the service control manager.
	* @param hService - Handle to the service to be stopped.
	* @param fStopDependencies - Indicates whether to stop dependent services.
	* @param dwTimeout - maximum time (in milliseconds) to wait
	*
	* @return If the operation is successful, returns ERROR_SUCCESS. Otherwise, returns a system error code.
	*/
	DWORD StopService( SC_HANDLE hSCM, SC_HANDLE hService, BOOL fStopDependencies, DWORD dwTimeout );
private:
	SC_HANDLE schSCManager;   ///<NT���������
	SC_HANDLE schService;     ///<NT������
};
