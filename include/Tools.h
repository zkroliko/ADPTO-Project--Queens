#ifndef ADPTO_TOOLS_H
#define ADPTO_TOOLS_H

template <typename M, typename V>
void mapToVec( const  M & m, V & v ) {
    for( typename M::const_iterator it = m.begin(); it != m.end(); ++it ) {
    	v.push_back( it->second );
    }
}
#endif //ADPTO_TOOLS_H
