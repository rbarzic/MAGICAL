/**
 * @file Terminal.h
 * @brief Class for a terminal, a collection of pins
 * @author anoynmous anoynmous
 * @date 12/07/2018
 */

#ifndef AROUTER_TERMINAL_H_
#define AROUTER_TERMINAL_H_

#include "NetNode.h"

PROJECT_NAMESPACE_BEGIN

/// @class AROUTER::Terminal
/// @brief a collection of pins, each terminal is belonging to one net
class Terminal
{
    public:
        explicit Terminal () = default;
        /*------------------------------*/ 
        /* Getters                      */
        /*------------------------------*/ 
        /// @brief get the pin array
        /// @return array of pins belonging to this terminal
        const std::vector<IndexType> & pinArray() const { return _pinArray; }
        /// @brief get the pin array
        /// @return array of pins belonging to this terminal
        std::vector<IndexType> & pinArray() { return _pinArray; }
        /// @brief number of pins
        /// @return number of pins
        IndexType numPins() const { return _pinArray.size(); }
        /// @brief get a pin
        /// @param the index of the pin in this terminal
        /// @return the index of the pin in the database
        IndexType pin(IndexType idx) const { return _pinArray.at(idx); }
        /// @brief get the array of intraCellConnection
        /// @return the list of shapes representing the intra-cell connection
        const std::vector<BoxLayer> & intraCellRoute() const { return _intraCellRoute; }
        /// @brief get number of intra cell connection shapes
        /// @return number of intra cell connection shapes
        IndexType numIntraShapes() const { return _intraCellRoute.size(); }
        /// @brief get the shape of a intra cell connection rectangle
        /// @param the index of the rectangle
        /// @return a intra cell connection rectangle
        const BoxLayer & intraShape(IndexType shapeIdx) const { return _intraCellRoute.at(shapeIdx); }
        /// @brief determine if a intra cell connection rectangle should be used as outer connection
        /// @return if true: this shape can be used as connecting points for the outerside. if false: this should be seemed as a blockage for the router
        bool isIntraShapeConnect(IndexType shapeIdx) const { return _allowedForConnection.at(shapeIdx); }
        /// @brief get the primary search point
        /// @return the primary search points used in the routing
        const NetNodeLoc & primarySearchPoint() const { return _primarySearchPoint; }
        /// @brief get the primary search point shape
        /// @return the index of the shape the primary search point belonging to
        IndexType primarySearchPointShape() const { return _primaryShapeIdx; }
        /// @brief get other search points
        const std::vector<NetNodeLoc> & otherSearchPoints() const { return _otherSearchPoints; }
        /// @brief get whether the shape is generated by the router or part of the placment 
        bool generated(IndexType shapeIdx) const { return _generated.at(shapeIdx); }
        /// @brief get the bounding box for the terminal
        /// @return the bounding box for the terminal
        const Box<LocType> & bbox() const { return _bbox; }
        /*------------------------------*/ 
        /* Adder                        */
        /*------------------------------*/ 
        /// @brief add pin
        void addPin(IndexType pinIdx) { _pinArray.emplace_back(pinIdx); }
        /// @brief add an intraCellShape
        /// @param box: the box of shape. layer: the index of layer (all layers) allowedForConnect: whether this shape can be used for connection in routing
        void addIntraBox(const Box<LocType> &box, IndexType layer, bool allowedForConnect, bool generated) { _intraCellRoute.emplace_back(box, layer); _allowedForConnection.emplace_back(allowedForConnect); _generated.emplace_back(generated);  }
        /// @brief add other search point
        /// @param the search point and shapeIdx
        void addOtherSearchPoint(const NetNodeLoc &loc, IndexType shapeIdx) { _otherSearchPoints.emplace_back(loc); _searchPointShapeIdx.emplace_back(shapeIdx); }
        /*------------------------------*/ 
        // Handle the small shape between horizontal pin segment and fingers
        /* Setters                      */
        /*------------------------------*/ 
        /// @brief set the primary search point
        /// @param primarySearchPoint: the primary search points used in the routing
        void setPrimarySearchPoint(const NetNodeLoc &primarySearchPoint) { _primarySearchPoint = primarySearchPoint; }
        /// @brief set the shape index for the primary search point
        /// @param shapeIdx: the shape index this primary search point belonging to
        void setPrimarySearchPointShape(IndexType shapeIdx) { _primaryShapeIdx = shapeIdx; }
        /// @brief calculate the bounding box
        void findBBox();
        /*------------------------------*/ 
        /* Inquery                      */
        /*------------------------------*/ 
        /// @brief determine whether a point is inside the connectable pin of this terminal
        /// @param a point representing the inquery rectangle
        /// @param the layer of the point, allLayer/RouterLayer
        /// @return whether this point is valid within the connection of this terminal
        bool isPointConnected(XY<LocType> loc, IndexType allLayer) const
        {
            for (IndexType idx = 0; idx < this->numIntraShapes(); ++idx)
            {
                if (this->isIntraShapeConnect(idx) && this->intraShape(idx).layer() == allLayer)
                {
                    if (this->intraShape(idx).box().contain(loc))
                    {
                        return true;
                    }
                }
            }
            return false;
        }
    private:
        std::vector<IndexType> _pinArray; ///< The pins that belonging to this terminal
        std::vector<BoxLayer> _intraCellRoute; ///< The intra-cell connection of the terminal
        NetNodeLoc _primarySearchPoint; /// The primary search points
        IndexType _primaryShapeIdx; ///< The shapeIdx for the primary search point
        std::vector<NetNodeLoc> _otherSearchPoints; ///< Allowed search start points
        std::vector<IndexType> _searchPointShapeIdx; ///< Shape index for the other search points
        std::vector<bool> _allowedForConnection; ///< If true, this shape is allowed for connection. If false, it is regarded as blockage in the routing
        std::vector<bool> _generated; /// If true, this is part of interconnection, if false, it is part of placement
        Box<LocType> _bbox; ///< The bounding box for the terminal
};

inanoynmouse void Terminal::findBBox()
{
    if (_intraCellRoute.size() == 0)
    {
        _bbox = Box<LocType>(0, 0, 0, 0);
    }
    _bbox = _intraCellRoute.front().box();
    for (IndexType idx = 1; idx < _intraCellRoute.size(); ++idx)
    {
        _bbox.unionBox(_intraCellRoute.at(idx).box());
    }
}

PROJECT_NAMESPACE_END

#endif ///AROUTER_TERMINAL_H_