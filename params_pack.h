#ifndef PARAMS_PACK_H_
#define PARAMS_PACK_H_
template <typename ...>
struct sizer;

template <>
struct sizer<>{
   const static unsigned int size = 0;
};

template <typename T1, typename ...T>
struct sizer<T1,T...>{
	const static unsigned int size = 1 + sizer<T...>::size;
};


template <typename ...>
struct first_typer;

template <>
struct first_typer<>{
   typedef void first_type;
};

template <typename T1, typename ...T>
struct first_typer<T1,T...>{
	 typedef T1 first_type;
};


template <typename ...>
struct last_typer;

template <>
struct last_typer<>{
   typedef void last_type;
};

template <typename T>
struct last_typer<T>{
   typedef T last_type;
};

template <typename T1, typename ...T>
struct last_typer<T1, T... >
    : public last_typer<T...>{
};


template <typename ...>
struct indexer;

template <>
struct indexer<>{
    template <unsigned int>
    struct at{
      typedef void type;
    };
};

template <typename T1, typename ...T>
struct indexer<T1, T...> {
	template <unsigned int index, typename dummy = void>
    struct at
        :public indexer<T...>::template at<index - 1>{
    };

    template <typename dummy> //explicit specialization is disabled so we choose partial specialization
    struct at<0, dummy>{
        typedef T1 type;
    };
};

template <typename ... T>
struct pop_fronter;

template <>
struct pop_fronter<>{
    template <template <typename ...> class C>
    struct pop_front{
        typedef C<> type;
    };
};

template <typename T1, typename ... T>
struct pop_fronter<T1, T...>{
    template <template <typename ...> class C>
    struct pop_front{
        typedef C<T...> type;
    };
};


template <int steps, typename ... T >
struct headtotailer_impl;

template <int steps, typename T1, typename ... T >
struct headtotailer_impl<steps, T1, T...>
       :public headtotailer_impl<(steps > 0 ? (steps - 1) : 0), T..., T1 >{
};

template < typename T1, typename ... T >
struct headtotailer_impl<0, T1, T...>{
    template <template <typename ...> class C>
    struct headtotail{
        typedef C<T1, T...> type;
    };
};

template <int steps>
struct headtotailer_impl<steps>{
    template <template <typename ...> class C>
    struct headtotail{
        typedef C<> type;
    };
};


template <typename ... T >
struct headtotailer{
    template <template <typename ...> class C, int steps = (sizer<T...>::size > 0) ? (sizer<T...>::size - 1) : 0>
    struct headtotail
        :public headtotailer_impl<steps, T...>::template
                headtotail<C>{
    };
};


template <typename ... T>
struct pop_backer{
    template <template <typename ...> class C>
    struct pop_back
        :public headtotailer<T...>::template headtotail
                <pop_fronter>::type::template pop_front
                <C>{
    };
};

template <>
struct pop_backer<>{
    template <template <typename ...> class C>
    struct pop_back{
        typedef C<> type;
    };
};


template <typename ... T>
struct ranger;

template <typename ... T>
struct ranger{
    const static int end_index = sizer<T...>::size;
    template <template <typename ...> class C,  int begin = 0, int end = end_index, int input_check =
        (begin < end && end_index > 0 && end > 0 ) ?
            ((begin >= 0 && end <= end_index) ? 0:  //good input
             1) :                                   //input with some correctable errors
            -1                                      //bad input
    >
    struct range;

    template <template <typename ...> class C,  int begin , int end >
    struct range<C, begin, end, 0>:
        public  pop_fronter<T...>::template pop_front
                <ranger>::type::template range<C, begin - 1, end - 1, 0>
    {};

    template <template <typename ...> class C,  int begin , int end >
    struct range<C, begin, end, 1>:
        public  range<C, ((begin >= 0) ? begin : 0) ,  ((end <= end_index) ? end : end_index), 0>
    {};

    template <template <typename ...> class C,  int begin , int end >
    struct range<C, begin, end, -1>{
        typedef C<> type;
    };

    template <template <typename ...> class C,  int end >
    struct range<C, 0, end, 0>:
        public  pop_backer<T...>::template pop_back
                <ranger>::type::template range
                <C, 0, (end > 0 ? end : 0) >{
    };

    template <template <typename ...> class C>
    struct range<C, 0, sizer<T...>::size, 0>{
        typedef C<T...> type;
    };
};


template <typename ... T>
struct concater{
    template <template <typename ...> class C, typename ... R>
    struct concate{
        typedef C<T..., R...> type;
    };

    template <template <typename ...> class C, typename ... R>
    struct rconcat{
        typedef C< R..., T...> type;
    };

    template <template <typename ...> class C, typename R >
    struct concate_concater;

    template <template <typename ...> class C, typename ...R >
    struct concate_concater<C, concater<R...> >{
         typedef C< T..., R...> type;
    };

    template <template <typename ...> class C, typename R >
    struct rconcate_concater;

    template <template <typename ...> class C, typename ...R >
    struct rconcate_concater<C, concater<R...> >{
         typedef C< R..., T...> type;
    };
};


template <typename ... T>
struct eraser{
    template <template <typename ...> class C, int pos>
    struct erase:
        public
            ranger<T...>::template
                range<concater,  0,  pos >::type::template
            concate_concater<C, typename ranger<T...>::template
                range<concater, pos + 1,  sizer<T...>::size>::type >
    {};
};

template <typename ... T>
struct swapper{
    template <template <typename ...> class C, int lhs, int rhs>
    struct swap:
        public
            ranger<T...>::template
                range<concater,  0,  lhs >::type::template
            concate<concater, typename indexer<T...>::template
                at<rhs>::type >::type::template
            concate_concater<concater, typename ranger<T...>::template
                range<concater, lhs + 1,  rhs >::type >::type::template
            concate<concater, typename indexer<T...>::template
                at<lhs>::type >::type::template
            concate_concater<C, typename ranger<T...>::template
                range<concater, rhs + 1, sizer<T...>::size>::type >
    {};
};


template <typename ... T>
struct reverser{
    template <template <typename ...> class C>
    struct reverse{
        const static int size = sizer<T...>::size;
        const static int pivot = size / 2;
        typedef typename ranger<T...>::template
                range<reverser,  pivot,  size >::type::template
                reverse<concater>::type::template
            concate_concater<C,
                typename ranger<T...>::template range<reverser,  0,  pivot >::type::template
                reverse<concater>::type>::type type;
    };
};

template <>
struct reverser<>{
    template <template <typename ...> class C>
    struct reverse{
       typedef C<> type;
    };
};

template <typename T>
struct reverser<T>{
    template <template <typename ...> class C>
    struct reverse{
       typedef C<T> type;
    };
};


template <typename ... T>
struct params_pack:
    public sizer<T...>,

    public first_typer<T...>,
    public last_typer<T...>,
    public indexer<T...>,

    public pop_fronter<T...>,
    public pop_backer<T...>,
    public ranger<T...>,
    public concater<T...>,
    public eraser<T...>,
    public swapper<T...>,
    public reverser<T...>
{
};

#endif //PARAMS_PACK_H_
