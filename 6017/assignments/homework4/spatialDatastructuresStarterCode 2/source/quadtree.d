import common;
   
alias Point!Dim = Point; 
alias AABB!Dim = AABB;
alias P2 = Point!2;


class quadTreeNode{
    AABB aabb; 
    Point[] points;
    quadTreeNode* NW;
    quadTreeNode* NE;
    quadTreeNode* SW;
    quadTreeNode* SE;
    bool isLeaf = false;

    this(Point[] points, AABB aabb){
        this.NW = null;
        this.NE = null;
        this.SW = null;
        this.SE = null;
      

        //threshold is '4' used to determine if we should continuously keep splitting 
        if(points.length < 4){
            this.point = points;
            isLeaf = true; 
        }
        else{
    
            //recursive   
            auto boundary = boundingBox(points);
            roots.aabb = boundary;

            auto midX = (boundary.max[0] + boundary.min[0]) / 2;
            auto midY = (boundary.max[1] + boundary.min[1]) / 2;

            auto rightHalfPoints = points.partitionByDimension!0((midX);
            auto leftHalfPoints = points[0 .. $ - rightHalfPoints.length];

            //left side
            auto topLeftPoints = leftHalfPoints.partitionByDimension!1((midY);
            auto bottomLeftPoints = leftHalfPoints[0 .. $ - topHalfPoints.length];

            //right side
            auto topRightPoints = rightHalfPoints.partitionByDimension!1(midY);
            auto bottomRightPoints = rightHalfPoints[0 .. & - topRightPoints.length];

            //creating the bounding boxes of those new quadrants found
            AABB topLeftBox;
            topLeftBox.min = Point([boundary.min[0], midY]);
            topLeftBox.max = Point([midX, boundary.max[1]]);
            
            AABB topRightBox;
            topRightBox.min = Point([midX, midY]);
            topRightBox.max = Point([boundary.max[0], boundary.max[1]]);
            
            AABB bottomLeftBox;
            bottomLeftBox.min = Point([boundary.min[0], boundary.min[1]]);
            bottomLeftBox.max = Point([midX, midY]);
            
            AABB bottomRightBox;
            bottomRightBox.min = Point([midX, boundary.min[1]]);
            bottomRightBox.max = Point([boundary.max[0], midY]);
            //making children for this node, each of those is a new node with a new quarter and points in that array
            //nw is build a new node of out list of nodes in top left corner etc..
            //this calls the constructor for the top left values
            this.NW = new quadTreeNode(topLeftPoints, topLeftBox);
            this.NE = new quadTreeNode(topRightPoints, topRightBox);
            this.SW = new quadTreeNode(botLeftPoints, bottomLeftBox);
            this.SE = new quadTreeNode(botRightPoints, bottomRightBox);

        }
    }
}

struct quadTree(size_t dim){
    //should call the recursive node constructor
    quadTreeNode* root;

     this(Point[] points) {
        root = new quadTreeNode(points, boundingBox(points));
    }

}

P2[] rangeQuery(P2 p, float r){
    P2[] ret;
  
    void recurseRangeQuery(quadTreeNode n){
        //checking if node's aabb is outside of the radius
        P2 closestPoint = closest(n.aabb, p);
        if(distance(closestPoint , p) <= r){
            if(n.isLeaf){
                //iterate through the points and check if they are within r to p
                foreach(point ; n.points){
                    if(distance(point , p) <= r){
                        ret ~= point;
                    }
                }
            }
            else{
            //if not a leaf, recurse children
            if (n.NW !is null) recurseRangeQuery(n.NW);
            if (n.NE !is null) recurseRangeQuery(n.NE);
            if (n.SW !is null) recurseRangeQuery(n.SW);
            if (n.SE !is null) recurseRangeQuery(n.SE);
            }
        }
    }
    recurseRangeQuery(root);
    return ret;
}





//knn use sort by distance 
P2[] KNNQuery(P2 p, size_t k, quadTreeNode root){
    P2[] ret;
    auto pq = makePriorityQueue(p);

    void recurseKNNQuery(quadTreeNode n){
        if(n.isLeaf){
            foreach (point; n.points){
                pq.insert(point);
                
                //sorting the points in the pq by distance to the p
                sortByDistance(pq, p);
                // if pq has more than k nearest neighbors, remove furthest point
                if(pq.length > k){
                    pq.popBack();
                }
            }
        } else{
            //calc closest point within aabb to the query point
            P2 closestPoint = closest(n.aabb, p);
           // Recurse into the child node containing closestPoint
            if (closestPoint[0] <= n.aabb.min[0] + (n.aabb.max[0] - n.aabb.min[0]) / 2) {
                if (closestPoint[1] <= n.aabb.min[1] + (n.aabb.max[1] - n.aabb.min[1]) / 2) {
                    if (n.SW !is null) recurseKNNQuery(n.SW);
                } else {
                    if (n.NW !is null) recurseKNNQuery(n.NW);
                }
            } else {
                if (closestPoint[1] <= n.aabb.min[1] + (n.aabb.max[1] - n.aabb.min[1]) / 2) {
                    if (n.SE !is null) recurseKNNQuery(n.SE);
                } else {
                    if (n.NE !is null) recurseKNNQuery(n.NE);
                }
            }
        }
    }

    recurseKNNQuery(root);

    // Get the k nearest neighbors from the priority queue
    while (!pq.empty && ret.length < k) {
        ret ~= pq.popFront();
    }

    return ret;
}