#pragma once

#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <cctype>
#include <memory>
#include <ctime>
#include <list>
#include <set>
#include <map>
#ifdef _MSC_VER
#pragma warning (disable : 4996)
#endif

namespace vcd {
namespace utils {
// -----------------------------
std::string format(const char *fmt, ...);
std::string now();
}

// -----------------------------
// valid VCD scope types
enum class ScopeType : char
{ begin, fork, function, module, task };
// valid VCD variable types
enum class VariableType : char
{ wire, reg, string, parameter, integer, real, realtime, time, event,
  supply0, supply1, tri, triand, trior, trireg, tri0, tri1, wand, wor };
// valid timescale numbers
enum class TimeScale : char
{ ONE = 1, TEN = 10, HUNDRED = 100 };
// valid timescale units
enum class TimeScaleUnit : char
{ s, ms, us, ns, ps, fs };
// -----------------------------
enum VCDValues : char
{ ONE='1', ZERO='0', UNDEF='x', HIGHV='z' };

using TimeStamp = unsigned;
using VarValue = std::string;

// -----------------------------
class VCDException : public std::exception
{
public:
    explicit VCDException(const std::string & message) : m_message("VCD error: " + message) {}
    virtual const char* what() const throw () { return m_message.c_str(); }

private:
    const std::string m_message;
};

class VCDPhaseException : public VCDException
{
public:
    explicit VCDPhaseException(const std::string& message) : VCDException(message) {}
};

class VCDTypeException : public VCDException
{
public:
    explicit VCDTypeException(const std::string& message) : VCDException(message) {}
};

// -----------------------------
struct VCDScope;
using ScopePtr = std::shared_ptr<VCDScope>;
struct ScopePtrHash
{ bool operator()(const ScopePtr &l, const ScopePtr &r) const; };

// -----------------------------
class VCDVariable;
using VarPtr = std::shared_ptr<VCDVariable>;
struct VarPtrHash
{ size_t operator()(const VarPtr &p) const; };
struct VarPtrEqual
{ bool operator()(const VarPtr &a, const VarPtr &b) const; };

// -----------------------------
struct VarSearch;
using VarSearchPtr = std::shared_ptr<VarSearch>;

// -----------------------------
struct VCDHeader;
struct VCDHeaderDeleter { void operator()(VCDHeader *p); };
using HeadPtr = std::unique_ptr<VCDHeader, VCDHeaderDeleter>;

// -----------------------------
HeadPtr makeVCDHeader(TimeScale     timescale_quan = TimeScale::ONE,
                      TimeScaleUnit timescale_unit = TimeScaleUnit::ns,
                      const std::string& date = utils::now(),
                      const std::string& comment = "",
                      const std::string& version = "");

// -----------------------------
// Writer of a Value Change Dump file
// A VCD file captures time-ordered changes to the value of variables
class VCDWriter
{
    FILE *_ofile;
    TimeStamp _timestamp;
    HeadPtr _header;

    // settings
    std::string _filename;
    std::string _scope_sep;
    ScopeType   _scope_def_type;

    std::set<ScopePtr, ScopePtrHash> _scopes;
    std::unordered_set<VarPtr, VarPtrHash, VarPtrEqual> _vars;

    // check changes of vars' values
    std::unordered_map<VarPtr, VarValue> _vars_prevs;

    // state
    bool _closed;
    bool _dumping;
    bool _registering;
    // gen var idents (internal names)
    unsigned   _next_var_id;
    VarSearchPtr _search;

public:
    VCDWriter(const std::string &filename, HeadPtr &header, unsigned init_timestamp = 0u);

    ~VCDWriter()
    {
        flush();
        fclose(this->_ofile);
    }

    // Register a VCD variable and return its mark to change value further.
    // Remember, all VCD variables must be registered prior to any value changes.
    // Note, *size* may be `0`, some types ("int", "real", "event") have a default size
    VarPtr register_var(const std::string &scope,                  // Variable belongs within the hierarchical scope
                        const std::string &name,                   // Human-readable variable idetifier
                        VariableType type = var_def_type,          // Verilog data type of variable
                        unsigned size = 0,                         // Size of variable, in bits
                        const VarValue &init = {VCDValues::UNDEF}, // Initial value (optional)
                        bool duplicate_names_check = true);        // speed-up (optimisation)

    // Change variable's value in VCD stream.
    // Call this method, for all variables changed on this *timestamp*.
    // It is okay to call it multiple times with the same *timestamp*, 
    // but never call with a past *timestamp*
    // Return:  *true* if new_value is dumped into VCD file,
    //         *false* if new_value is not changed from priveios *timestamp* for a given var
    bool change(VarPtr var, TimeStamp timestamp, const VarValue &value)
    { return _change(var, timestamp, value, false); }

    bool change(const std::string &scope, const std::string &name, TimeStamp timestamp, const VarValue &value);

    // Suspend dumping to VCD file
    void dump_off(TimeStamp current)
    {
        if (_dumping && !_registering && _vars_prevs.size())
            _dump_off(current);
        _dumping = false;
    }
    // Resume dumping to VCD file
    void dump_on(TimeStamp current)
    {
        if (!_dumping && !_registering && _vars_prevs.size())
            fprintf(_ofile, "#%d", current);
        _dump_values("$dumpon");
        _dumping = true;
    }

    // Flush any buffered VCD data to output file.
    // If the VCD header has not already been written, calling `flush()` will force
    // the header to be written thus disallowing any further variable registrations.
    void flush(const TimeStamp *current = NULL)
    {
        if (_closed)
            throw VCDPhaseException{ "Cannot flush() after close()" };
        if (_registering)
            _finalize_registration();
        if (current != NULL && *current > _timestamp)
            fprintf(_ofile, "#%d", *current);
        fflush(_ofile);
    }
    // Close VCD writer. Any buffered VCD data is flushed to the output file.
    // After `close()`, NO variable registration or value changes will be accepted.
    // Note, the output file-stream will be closed in destructor of `VCDWriter`
    void close(const TimeStamp *final = NULL)
    {
        if (_closed) return;
        flush(final);
        _closed = true;
    }

    //! VCD viewer applications may display different scope types differently
    void set_scope_type(std::string& scope, ScopeType);

    void set_scope_default_type(ScopeType new_type)
    { _scope_def_type = new_type; }

    void set_scope_sep(const std::string& scope_sep)
    {
        if (scope_sep.size() == 0 || scope_sep == _scope_sep)
            return;
        _scope_sep = scope_sep;
    }
    //! get VCD Variable (if it is registered var() != NULL)
    VarPtr var(const std::string &scope, const std::string &name) const;

    static const VariableType var_def_type = VariableType::wire;

protected:
    bool _change(VarPtr, TimeStamp, const VarValue&, bool);
    void _dump_off(TimeStamp);
    void _dump_values(const std::string& keyword);
    void _scope_declaration(const std::string& scope, size_t sub_beg, size_t sub_end = std::string::npos);
    //! Dump VCD header into file
    void _write_header();
    //! Turn to dumping phase, no more variables regestration allowed
    void _finalize_registration();
};

// -----------------------------
using WriterPtr = std::shared_ptr<VCDWriter>;

}
