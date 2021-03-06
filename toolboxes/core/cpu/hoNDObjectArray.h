/** \file   hoNDObjectArray.h
\brief  CPU-based N-dimensional array for object pointers
if delete_data_on_destruct == true, the object will be released; otherwise, only the object array memory is released
\author Hui Xue
*/

#pragma once

#include "hoNDArray.h"

namespace Gadgetron
{

    template <typename TObjectType> class hoNDObjectArray : public hoNDArray<TObjectType*>
    {
    public:

        typedef hoNDArray<TObjectType*> BaseClass;
        typedef float coord_type;
        typedef typename BaseClass::value_type value_type;

        hoNDObjectArray();

        explicit hoNDObjectArray(std::vector<size_t> &dimensions);
        explicit hoNDObjectArray(std::vector<size_t> *dimensions);
        explicit hoNDObjectArray(boost::shared_ptr< std::vector<size_t> > dimensions);

        virtual ~hoNDObjectArray();

        // Copy constructors
        hoNDObjectArray(const hoNDObjectArray<TObjectType> &a);
        explicit hoNDObjectArray(const hoNDObjectArray<TObjectType> *a);

        // Assignment operator
        hoNDObjectArray& operator=(const hoNDObjectArray& rhs);

        virtual void create(std::vector<size_t>& dimensions);
        virtual void create(std::vector<size_t> *dimensions);
        virtual void create(boost::shared_ptr< std::vector<size_t> > dimensions);

        virtual void print(std::ostream& os) const;

        // deep copy
        void copyFrom(const hoNDObjectArray<TObjectType>& aArray);

    protected:

        using BaseClass::dimensions_;
        using BaseClass::offsetFactors_;
        using BaseClass::data_;
        using BaseClass::elements_;
        using BaseClass::delete_data_on_destruct_;
    };

    template <typename TObjectType> 
    hoNDObjectArray<TObjectType>::hoNDObjectArray() : BaseClass() 
    {
    }

    template <typename TObjectType> 
    hoNDObjectArray<TObjectType>::hoNDObjectArray(std::vector<size_t> *dimensions) : BaseClass(dimensions)
    {
        this->create(dimensions);
    }

    template <typename TObjectType> 
    hoNDObjectArray<TObjectType>::hoNDObjectArray(std::vector<size_t> &dimensions) : BaseClass(dimensions)
    {
        this->create(dimensions);
    }

    template <typename TObjectType> 
    hoNDObjectArray<TObjectType>::hoNDObjectArray(boost::shared_ptr< std::vector<size_t> > dimensions) : BaseClass(dimensions)
    {
        this->create(dimensions);
    }

    template <typename TObjectType> 
    hoNDObjectArray<TObjectType>::~hoNDObjectArray()
    {
        if (this->data_ != NULL)
        {
            size_t n;
            for (n = 0; n < this->elements_; n++)
            {
                if (this->data_[n] != NULL)
                {
                    delete this->data_[n];
                    this->data_[n] = NULL;
                }
            }
        }

        if (this->delete_data_on_destruct_)
            this->deallocate_memory();
    }

    template <typename TObjectType> 
    hoNDObjectArray<TObjectType>::hoNDObjectArray(const hoNDObjectArray<TObjectType>  *a) : BaseClass(a)
    {
        this->copyFrom(*a);
    }

    template <typename TObjectType> 
    hoNDObjectArray<TObjectType>::hoNDObjectArray(const hoNDObjectArray<TObjectType> &a) : BaseClass(a)
    {
        this->copyFrom(a);
    }

    template <typename TObjectType> 
    hoNDObjectArray<TObjectType>& hoNDObjectArray<TObjectType>::operator=(const hoNDObjectArray<TObjectType>& rhs)
    {
        if ( &rhs == this ) return *this;

        this->copyFrom(rhs);

        return *this;
    }

    template <typename TObjectType> 
    void hoNDObjectArray<TObjectType>::create(std::vector<size_t>& dimensions)
    {
        BaseClass::create(dimensions);

        for ( size_t n=0; n<this->elements_; n++ )
        {
            this->data_[n] = NULL;
        }
    }

    template <typename TObjectType> 
    void hoNDObjectArray<TObjectType>::create(std::vector<size_t> *dimensions)
    {
        BaseClass::create(dimensions);

        for ( size_t n=0; n<this->elements_; n++ )
        {
            this->data_[n] = NULL;
        }
    }

    template <typename TObjectType> 
    void hoNDObjectArray<TObjectType>::create(boost::shared_ptr< std::vector<size_t> > dimensions)
    {
        BaseClass::create(dimensions);

        for ( size_t n=0; n<this->elements_; n++ )
        {
            this->data_[n] = NULL;
        }
    }

    template <typename TObjectType>
    void hoNDObjectArray<TObjectType>::copyFrom(const hoNDObjectArray<TObjectType>& aArray)
    {
        try
        {
            if (!this->dimensions_equal(&aArray))
            {
                this->create(aArray.get_dimensions());
            }

            long long i;
            for (i = 0; i<(long long)elements_; i++)
            {
                if (aArray(i) != NULL)
                    data_[i] = new TObjectType(*aArray(i));
                else
                    data_[i] = NULL;
            }

            this->delete_data_on_destruct_ = true;
        }
        catch (...)
        {
            GADGET_THROW("Exceptions happened in hoNDObjectArray::copyFrom(...) ... ");
        }
    }

    template <typename TObjectType> 
    void hoNDObjectArray<TObjectType>::print(std::ostream& os) const
    {
        using namespace std;

        os.unsetf(std::ios::scientific);
        os.setf(ios::fixed);

        os << "-------------- Gagdgetron ND Object Array -------------" << endl;
        this->printContent(os);
    }
}
