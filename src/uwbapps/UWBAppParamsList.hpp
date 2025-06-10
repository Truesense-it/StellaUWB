// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Truesense Srl

#ifndef UWBAPPPARAMSLIST_HPP
#define UWBAPPPARAMSLIST_HPP


#include "hal/uwb_types.hpp"

template <typename T, typename P1, typename P2, typename P3> class UWBAppParamsList {
public:
    UWBAppParamsList() : _size(0) {}

 
    bool addOrUpdateParam(P1 param_id, P2 param_type, P3 param_value, uint16_t param_len=0) {
        
        // First, check if the parameter already exists
        for (unsigned int i = 0; i < _size; i++) {
            if (_paramsList[i].param_id == param_id) {
                // Parameter exists, update it
                _paramsList[i].param_type = param_type;
                _paramsList[i].param_value = param_value;
                //UWBHAL.Log_D("param updated %x: %x %x",param_id,param_type,param_value);
                return true; // Update successful
            }
        }
        
            

        // If the parameter does not exist, add it (if there's space)
        if (_size < MAX_SIZE) {
            _paramsList[_size].param_id = param_id;
            _paramsList[_size].param_type = param_type;
            _paramsList[_size].param_value = param_value;
            _size++;
            //UWBHAL.Log_D("param added");
            return true; // Addition successful
        } else {
            // Array is full, cannot add new parameter
            //UWBHAL.Log_E("too many params");
            return false;
        }
    }

    bool addOrUpdateParam(T param)
    {
        // First, check if the parameter already exists
        for (unsigned int i = 0; i < _size; i++) {
            if (_paramsList[i].param_id == param.param_id) {
                // Parameter exists, update it
                _paramsList[i].param_type = param.param_type;
                _paramsList[i].param_value = param.param_value;
                //UWBHAL.Log_D("param updated");
                return true; // Update successful
            }
        }

        // If the parameter does not exist, add it (if there's space)
        if (_size < MAX_SIZE) {
            _paramsList[_size].param_id = param.param_id;
            _paramsList[_size].param_type = param.param_type;
            _paramsList[_size].param_value = param.param_value;
            _size++;
            //UWBHAL.Log_D("param added");
            return true; // Addition successful
        } else {
            // Array is full, cannot add new parameter
            //UWBHAL.Log_D("List full");
            return false;
        }
    }

    

    bool removeParam(P1 param_id) {
        for (unsigned int i = 0; i < _size; i++) {
            if (_paramsList[i].param_id == param_id) {
                for (unsigned int j = i; j < _size - 1; j++) {
                    _paramsList[j] = _paramsList[j + 1];
                }
                _size--;
                return true;
            }
        }
        return false; // Param not found
    }

    T* findParam(P1 param_id) {
        for (unsigned int i = 0; i < _size; i++) {
            if (_paramsList[i].param_id == param_id) {
                return &_paramsList[i];
            }
        }
        return nullptr; // Param not found
    }

    // Method to return the raw paramsList array
    T* getParamsList() {
        return (T *)&_paramsList[0];
    }

    unsigned int getSize() {
        return _size;
    }
    

private:
    static const unsigned int MAX_SIZE = 30; // Maximum number of elements
    T _paramsList[MAX_SIZE];
    unsigned int _size; // Current number of elements
};

#endif //_UWBAPPPARAMSLIST_H_
