#ifndef IOKITD_IOOBJECT_H
#define IOKITD_IOOBJECT_H
#include <unordered_map>
#include <mach/mach.h>
#include <dispatch/dispatch.h>
#include <memory>

class IOObject
{
public:
	// If userOwned is true, the object will be deallocated when there are no senders left
	IOObject();
	virtual ~IOObject();

	virtual const char* className() const = 0;
	virtual bool conformsTo(const char* className);

	void retain();
	void release();
	void releaseLater();

	mach_port_t port() { return m_port; }

	// TODO: This should start adding a reference and all call sites should retain.
	// At the latest when we support objects that may go away (e.g. USB devices).
	static IOObject* lookup(mach_port_t port);
	static boolean_t deathNotify(mach_msg_header_t *request, mach_msg_header_t *reply);
private:
	mach_port_t m_port;
	dispatch_source_t m_dispatchSource = nullptr;
	static std::unordered_map<mach_port_t, IOObject*> m_objects;
};

#endif
