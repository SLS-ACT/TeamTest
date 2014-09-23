#ifndef __XROLEMANAGER_H__
#define __XROLEMANAGER_H__

class XRoleManager
{
public:
	XRoleManager(void);
	virtual ~XRoleManager(void);
public:
	static XRoleManager* shareInstance();

};
#endif // __XROLEMANAGER_H__

