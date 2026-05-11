#include <string>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <sstream>
#include <iostream>
#include "vcd_writer.h"


// -----------------------------
namespace vcd {
namespace utils {
void replace_new_lines(std::string &str, const std::string &sub);
}
using namespace utils;

// -----------------------------
struct VCDHeader final
{
    static const unsigned kws = 4;
    static const std::string kw_names[kws];

    TimeScale timescale_quan;
    TimeScaleUnit timescale_unit;
    std::string kw_values[kws];

    VCDHeader() = delete;
    VCDHeader(VCDHeader&&) = default;
    VCDHeader(const VCDHeader&) = delete;
    VCDHeader(TimeScale     timescale_quan,
              TimeScaleUnit timescale_unit,
              const std::string& date,
              const std::string& comment,
              const std::string& version) :
        timescale_quan{ timescale_quan },
        timescale_unit{ timescale_unit },
        kw_values{ timescale(), date, comment, version }
    {}

    std::string timescale() const
    {
        const std::string TIMESCALE_UNITS[] = { "s", "ms", "us", "ns", "ps", "fs" };
        return std::to_string(int(timescale_quan)) + " " + TIMESCALE_UNITS[int(timescale_unit)];
    }
};

// -----------------------------
HeadPtr makeVCDHeader(TimeScale timescale_quan, TimeScaleUnit timescale_unit, const std::string &date,
                      const std::string &comment, const std::string &version)
{
    return HeadPtr{ new VCDHeader(timescale_quan, timescale_unit, date, comment, version) };
}

// -----------------------------
const std::string VCDHeader::kw_names[VCDHeader::kws] = { "$timescale", "$date", "$comment", "$version" };

// -----------------------------
void VCDHeaderDeleter::operator()(VCDHeader *p) { delete p; }

// -----------------------------
struct VCDScope final
{
    std::string name;
    ScopeType   type;
    std::list<VarPtr> vars;

    VCDScope(const std::string &name, ScopeType type) : 
        name(name), type(type) {}
};

// -----------------------------
bool ScopePtrHash::operator()(const ScopePtr &l, const ScopePtr &r) const
{
    return (l->name < r->name);
}

// -----------------------------
// VCD variable details needed to call :meth:`VCDWriter.change()`.
class VCDVariable
{
    VCDVariable() = delete;
    VCDVariable(VCDVariable&&) = default;
    VCDVariable(const VCDVariable&) = delete;

protected:
    VCDVariable(const std::string &name, VariableType type, unsigned size, ScopePtr scope, unsigned next_var_id);

    std::string _ident;  // internal ID used in VCD output stream
    VariableType _type;  // VCD variable type, one of `VariableTypes`
    std::string  _name;  // human-readable name
    unsigned     _size;  // size of variable, in bits
    ScopePtr    _scope;  // pointer to scope string

    //! string representation of variable types
    static const std::string VAR_TYPES[];

public:
    //! string representation of variable declartion in VCD
    std::string declartion() const;
    //! string representation of value change record in VCD
    virtual VarValue change_record(const VarValue &value) const = 0;

    friend class VCDWriter;
    friend struct VarPtrHash;
    friend struct VarPtrEqual;
};

// -----------------------------
const std::string VCDVariable::VAR_TYPES[] = { 
    "wire", "reg", "string", "parameter", "integer", "real", "realtime", "time", "event",
    "supply0", "supply1", "tri", "triand", "trior", "trireg", "tri0", "tri1", "wand", "wor"
};

// -----------------------------
size_t VarPtrHash::operator()(const VarPtr &p) const
{
    std::hash<std::string> h;
    return (h(p->_name) ^ (h(p->_scope->name) << 1));
}

// -----------------------------
bool VarPtrEqual::operator()(const VarPtr &a, const VarPtr &b) const
{
    return (a->_name == b->_name) && (a->_scope->name == b->_scope->name);
}

// -----------------------------
// One-bit VCD scalar is a 4-state variable and thus may have one of
// `VCDValues`. An empty *value* is the same as `VCDValues::UNDEF`
struct VCDScalarVariable : public VCDVariable
{
    VCDScalarVariable(const std::string &name, VariableType type, unsigned size, ScopePtr scope, unsigned next_var_id) :
        VCDVariable(name, type, size, scope, next_var_id)
    {}
    VarValue change_record(const VarValue &value) const
    {
        char c = (value.size()) ? tolower(value[0]) : VCDValues::UNDEF;
        if (value.size() != 1 || (c != VCDValues::ONE   && c != VCDValues::ZERO
                               && c != VCDValues::UNDEF && c != VCDValues::HIGHV))
            throw VCDTypeException{ format("Invalid scalar value '%c'", c) };
        return {c};
    }
};

// -----------------------------
// String variable as known by GTKWave. Any `string` (character-chain) 
// can be displayed as a change.This type is only supported by GTKWave.
struct VCDStringVariable : public VCDVariable
{
    VCDStringVariable(const std::string &name, VariableType type, unsigned size, ScopePtr scope, unsigned next_var_id) :
        VCDVariable(name, type, size, scope, next_var_id)
    {}
    VarValue change_record(const VarValue &value) const
    {
        if (value.find(' ') != std::string::npos)
            throw VCDTypeException{ format("Invalid string value '%s'", value.c_str()) };
        return format("s%s ", value.c_str());
    }
};

// -----------------------------
// Real (IEEE-754 double-precision floating point) variable. Values must
// be numeric and can't be `VCDValues::UNDEF` or `VCDValues::HIGHV` states
struct VCDRealVariable : public VCDVariable
{
    VCDRealVariable(const std::string &name, VariableType type, unsigned size, ScopePtr scope, unsigned next_var_id) :
        VCDVariable(name, type, size, scope, next_var_id) {}
    std::string change_record(const VarValue &value) const
    { return format("r%.16g ", stod(value)); }
};

// -----------------------------
// Bit vector variable type for the various non-scalar and non-real 
// variable types, including integer, register, wire, etc.
struct VCDVectorVariable : public VCDVariable
{
    VCDVectorVariable(const std::string &name, VariableType type, unsigned size, ScopePtr scope, unsigned next_var_id) :
        VCDVariable(name, type, size, scope, next_var_id) {}
    std::string change_record(const VarValue &value) const;
};

// -----------------------------
struct VarSearch
{
    VCDScope vcd_scope = { "", ScopeType::module };
    ScopePtr ptr_scope = { &vcd_scope, [](VCDScope*) {} };
    VCDScalarVariable vcd_var = { "", VCDWriter::var_def_type, 0, ptr_scope, 0 };
    VarPtr ptr_var = { &vcd_var, [](VCDScalarVariable*) {} };

    VarSearch() = delete;
    VarSearch(ScopeType scope_def_type) : vcd_scope("", scope_def_type) {}
};

// -----------------------------
VCDWriter::VCDWriter(const std::string &filename, HeadPtr &header, unsigned init_timestamp) :
    _timestamp(init_timestamp),
    _header((header) ? std::move(header) : makeVCDHeader()),
    _filename(filename),
    _scope_sep("."),
    _scope_def_type(ScopeType::module),
    _closed(false),
    _dumping(true),
    _registering(true),
    _next_var_id(0),
    _search(std::make_shared<VarSearch>(_scope_def_type))
{
    if (!_header)
        throw VCDTypeException{ "Invalid pointer to header" };

    _ofile = std::fopen(_filename.c_str(), "w");
    if (!_ofile)
        throw VCDTypeException{ format("Can't open file '%s' for writing", _filename.c_str()) };
}

// -----------------------------
VarPtr VCDWriter::register_var(const std::string &scope, const std::string &name, VariableType type,
                               unsigned size, const VarValue &init, bool duplicate_names_check)
{
    VarPtr pvar;
    if (_closed)
        throw VCDPhaseException{ "Cannot register after close()" };
    if (!_registering)
        throw VCDPhaseException{ format("Cannot register new var '%s', registering finished", name.c_str()) };

    if (scope.size() == 0 || name.size() == 0)
        throw VCDTypeException{ format("Empty scope '%s' or name '%s'", scope.c_str(), name.c_str()) };

    _search->vcd_scope.name = scope;
    auto cur_scope = _scopes.find(_search->ptr_scope);
    if (cur_scope == _scopes.end())
    {
        auto res = _scopes.insert(std::make_shared<VCDScope>(scope, _scope_def_type));
        if (!res.second)
            throw VCDPhaseException{ format("Cannot insert scope '%s'", scope.c_str()) };
        cur_scope = res.first;
    }

    auto sz = [&size](unsigned def) { return (size ? size : def);  };

    VarValue init_value(init);
    switch (type)
    {
        case VariableType::integer:   
        case VariableType::realtime:
            if (sz(64) == 1)
                pvar = VarPtr(new VCDScalarVariable(name, type, 1, *cur_scope, _next_var_id));
            else
                pvar = VarPtr(new VCDVectorVariable(name, type, sz(64), *cur_scope, _next_var_id));
            break;

        case VariableType::real:
            pvar = VarPtr(new VCDRealVariable(name, type, sz(64), *cur_scope, _next_var_id));
            if (init_value.size() == 1 && init_value[0] == VCDValues::UNDEF)
                init_value = "0.0";
            break;

        case VariableType::string:
            pvar = VarPtr(new VCDStringVariable(name, type, sz(1), *cur_scope, _next_var_id));
            break;

        case VariableType::event:
            pvar = VarPtr(new VCDScalarVariable(name, type, 1, *cur_scope, _next_var_id));
            break;

        default:
            if (!size)
                throw VCDTypeException{ format("Must supply size for type '%s' of var '%s'",
                                               VCDVariable::VAR_TYPES[(int)type].c_str(), name.c_str()) };

            pvar = VarPtr(new VCDVectorVariable(name, type, size, *cur_scope, _next_var_id));
            if (init_value.size() == 1 && init_value[0] == VCDValues::UNDEF)
                init_value = std::string(size, VCDValues::UNDEF);
            break;
    }     
    if (type != VariableType::event)
        _change(pvar, _timestamp, init_value, true);
    
    if (duplicate_names_check && _vars.find(pvar) != _vars.end())
        throw VCDTypeException{ format("Duplicate var '%s' in scope '%s'", name.c_str(), scope.c_str()) };

    _vars.insert(pvar);
    (**cur_scope).vars.push_back(pvar);
    // Only alter state after change_func() succeeds
    _next_var_id++;
    return pvar;
}

// -----------------------------
bool VCDWriter::_change(VarPtr var, TimeStamp timestamp, const VarValue &value, bool reg)
{
    if (timestamp < _timestamp)
        throw VCDPhaseException{ format("Out of order value change var '%s'", var->_name.c_str()) };
    else if (_closed)
        throw VCDPhaseException{ "Cannot change value after close()" };

    if (!var)
        throw VCDTypeException{ "Invalid VCDVariable" };

    if (timestamp > _timestamp)
    {
        if (_registering)
            _finalize_registration();
        if (_dumping)
            fprintf(_ofile, "#%d\n", timestamp);
        _timestamp = timestamp;
    }

    std::string change_value = var->change_record(value);
    // if value changed
    auto it = _vars_prevs.find(var);
    if (it != _vars_prevs.end())
    {
        if (it->second == change_value)
            return false;
        it->second = change_value;
    }
    else
    {
        if (!reg)
            throw VCDTypeException{ format("VCDVariable '%s' do not registered", var->_name.c_str()) };
        else
            _vars_prevs.insert(std::make_pair(var, change_value));
    }
    // dump it into file
    if (_dumping && !_registering)
        fprintf(_ofile, "%s%s\n", change_value.c_str(), var->_ident.c_str());
    return true;
}

// -----------------------------
bool VCDWriter::change(const std::string &scope, const std::string &name, TimeStamp timestamp, const VarValue &value)
{
    return _change(var(scope, name), timestamp, value, false);
}

// -----------------------------
VarPtr VCDWriter::var(const std::string &scope, const std::string &name) const
{
    //!!! speed optimisation !!!
    //ScopePtr pscope = std::make_shared<VCDScope>(scope, _scope_def_type);
    //auto it_scope = _scopes.find(pscope);
    //if (it_scope == _scopes.end())
    //    throw VCDPhaseException{ format("Such scope '%s' does not exist", scope.c_str()) };
    //VarPtr pvar = std::make_shared<VCDScalarVariable>(name, VCDWriter::var_def_type, 0, *it_scope, 0);
    //auto it_var = _vars.find(pvar);
    _search->vcd_scope.name = scope;
    _search->vcd_var._name = name;
    auto it_var = _vars.find(_search->ptr_var);
    if (it_var == _vars.end())
        throw VCDPhaseException{ format("The var '%s' in scope '%s' does not exist", name.c_str(), scope.c_str()) };
    return *it_var;
}

// -----------------------------
void VCDWriter::set_scope_type(std::string &scope, ScopeType scope_type)
{
    ScopePtr pscope = std::make_shared<VCDScope>(scope, _scope_def_type);
    auto it = _scopes.find(pscope);
    if (it == _scopes.end())
        throw VCDPhaseException{ utils::format("Such scope '%s' does not exist", scope.c_str()) };
    (**it).type = scope_type;
}


// -----------------------------
void VCDWriter::_dump_off(TimeStamp timestamp)
{
    fprintf(_ofile, "#%d\n", timestamp);
    fprintf(_ofile, "$dumpoff\n");
    for (const auto &p : _vars_prevs)
    {
        const char *ident = p.first->_ident.c_str();
        const char *value = p.second.c_str();

        if (value[0] == 'r')
        {} // real variables cannot have "z" or "x" state
        else if (value[0] == 'b')
        { fprintf(_ofile, "bx %s\n", ident); }
        //else if (value[0] == 's')
        //{ fprintf(_ofile, "sx %s\n", ident); }
        else
        { fprintf(_ofile, "x%s\n", ident); }
    }
    fprintf(_ofile, "$end\n");
}

// -----------------------------
void VCDWriter::_dump_values(const std::string &keyword)
{
    fprintf(_ofile, "%s", (keyword + "\n").c_str());
    // TODO : events should be excluded
    for (const auto &p : _vars_prevs)
    {
        const char *ident = p.first->_ident.c_str();
        const char *value = p.second.c_str();
        fprintf(_ofile, "%s%s\n", value, ident);
    }
    fprintf(_ofile, "$end\n");
}

// -----------------------------
void VCDWriter::_scope_declaration(const std::string &scope, size_t sub_beg, size_t sub_end)
{
    const std::string SCOPE_TYPES[] = { "begin", "fork", "function", "module", "task" };

    auto scope_name = scope.substr(sub_beg, sub_end - sub_beg);
    auto scope_type = SCOPE_TYPES[int(_scope_def_type)].c_str();
    fprintf(_ofile, "$scope %s %s $end\n", scope_type, scope_name.c_str());
}

// -----------------------------
void VCDWriter::_write_header()
{
    for (unsigned int i = 0; i < VCDHeader::kws; ++i)
    {
        auto kwname = VCDHeader::kw_names[i];
        auto kwvalue = _header->kw_values[i];
        if (!kwvalue.size())
            continue;
        replace_new_lines(kwvalue, "\n\t");
        fprintf(_ofile, "%s %s $end\n", kwname.c_str(), kwvalue.c_str());
    }

    // nested scope
    size_t n = 0, n_prev = 0;
    std::string scope_prev = "";
    for (auto& s : _scopes) // sorted
    {
        const std::string &scope = s->name;
        // scope print close
        if (scope_prev.size())
        {
            n_prev = 0;
            n = scope_prev.find(_scope_sep);
            n = (n == std::string::npos) ? scope_prev.size() : n;
            // equal prefix
            while (std::strncmp(scope.c_str(), scope_prev.c_str(), n) == 0)
            {
                n_prev = n + _scope_sep.size();
                n = scope_prev.find(_scope_sep, n_prev);
                if (n == std::string::npos)
                    break;
            }
            // last
            if (n_prev != (scope_prev.size() + _scope_sep.size()))
                fprintf(_ofile, "$upscope $end\n");
            // close
            n = scope_prev.find(_scope_sep, n_prev);
            while (n != std::string::npos)
            {
                fprintf(_ofile, "$upscope $end\n");
                n = scope_prev.find(_scope_sep, n + _scope_sep.size());
            }
        }

        // scope print open
        n = scope.find(_scope_sep, n_prev);
        while (n != std::string::npos)
        {
            _scope_declaration(scope, n_prev, n);
            n_prev = n + _scope_sep.size();
            n = scope.find(_scope_sep, n_prev);
        }
        // last
        _scope_declaration(scope, n_prev);

        // dump variable declartion
        for (const auto& var : s->vars)
            fprintf(_ofile, "%s\n", var->declartion().c_str());

        scope_prev = scope;
    }

    // scope print close (rest)
    if (scope_prev.size())
    {
        // last
        fprintf(_ofile, "$upscope $end\n");
        n = scope_prev.find(_scope_sep);
        while (n != std::string::npos)
        {
            fprintf(_ofile, "$upscope $end\n");
            n = scope_prev.find(_scope_sep, n + _scope_sep.size());
        }
    }

    fprintf(_ofile, "$enddefinitions $end\n");
    // do not need anymore
    _header.reset(nullptr);
}

// -----------------------------
void VCDWriter::_finalize_registration()
{
    assert(_registering);
    _write_header();
    if (_vars_prevs.size())
    {
        fprintf(_ofile, "#%d\n", _timestamp);
        _dump_values("$dumpvars");
        if (!_dumping)
            _dump_off(_timestamp);
    }
    _registering = false;
}

// -----------------------------
VCDVariable::VCDVariable(const std::string &name, VariableType type, unsigned size, ScopePtr scope, unsigned next_var_id) :
    _type(type), _name(name), _size(size), _scope(scope)
{
    std::stringstream ss;
    ss << std::hex << next_var_id;
    _ident = ss.str();
}

// -----------------------------
std::string VCDVariable::declartion() const
{
    return format("$var %s %d %s %s $end", VAR_TYPES[int(_type)].c_str(), _size, _ident.c_str(), _name.c_str());
}

// -----------------------------
//  :Warning: *value* is string where all characters must be one of `VCDValues`.
//  An empty  *value* is the same as `VCDValues::UNDEF`
VarValue VCDVectorVariable::change_record(const VarValue &value) const
{
    if (value.size() > _size)
        throw VCDTypeException{ format("Invalid binary vector value '%s' size '%d'", value.c_str(), _size) };

    std::string val = ('b' + value + ' ');
    auto val_sz = value.size();
    
    for (auto i = 1u; i < (val_sz - 1); ++i)
    {
        char &c = val[i];
        c = tolower(val[i]);
        if (c != VCDValues::ONE && c != VCDValues::ZERO && c != VCDValues::UNDEF && c != VCDValues::HIGHV)
            throw VCDTypeException{ format("Invalid binary vector value '%s' size '%d'", val.c_str(), _size) };
    }

    if (!val_sz) val = ('b' + std::string(_size, VCDValues::UNDEF) + ' ');

    // align
    else if (val_sz < _size)
    {
        /***
         * Example: _size = 4, a 4 bit vector
         * value is 'xx' => val_sz = 2
         * then val is 'bxx ' when entering here, but this is not yet alligned
         * end result should look like 'b00xx '
         * so 'xx' needs to be aligned to the right
         * 'b|x|x| |'       val
         * 'b|0|0|x|x| |'   val desired
         * |0|1|2|3|4|5|    index positions
         */

        val.resize(_size + 2); // to allow full bit chars + 'b' in beginning and a space ' ' at the end
        auto k = _size - val_sz;
        /***
        * k: amount that original 'value' input in val (here 'xx') need to shifted
        * or in other words: number of zeros that need to be filled in before 'xx'
        * k is then size(4) - val_sz (2) = 2,
        * as seen in example first 'x' need to be moved from pos 1 -> 3, second from 2 -> 4
        * reverse through the 'xx' string, as otherwise chars might be moved to positions that need to be moved aswell
        * start at val_sz(2) and go back, ending before hitting the 'b'
        */
        for (auto i = val_sz ; i >= 1; --i)
            val[k + i] = val[i];
        // set remaining values zero
        for (auto i = 1u; i <= k; ++i)
            val[i] = VCDValues::ZERO;
        // set last char to a space
        val[_size+1] = ' ';
    }
    return val;
}

// -----------------------------
} //end namespace vcd

