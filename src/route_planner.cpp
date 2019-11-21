#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
	start_node = &m_model.FindClosestNode(start_x,start_y);
	end_node   = &m_model.FindClosestNode(end_x,end_y);

}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
	
	return (*node).distance(end_node); 
	/* Finds the heuristic(Eucledian distance)
	between the current node and the end_node.
	OR
	return node->distance(end_node);
	*/

}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
     
	
	 current_node->FindNeighbors(); // gives the neighboring nodes.
	 //neighbors- is a vector that stores all the neighboring nodes.
	 for(RouteModel::Node* neighbor_nodes : current_node->neighbors)
	 {
		 neighbor_nodes->parent   = current_node; //All the neighboring nodes must have its parent as the current_node.
		 neighbor_nodes->g_value  = current_node->g_value + (*current_node).distance(*neighbor_nodes);
		 neighbor_nodes->h_value  = CalculateHValue(*neighbor_nodes);
         open_list.push_back(neighbor_nodes); //Add the neighboring nodes to the open_list.
         neighbor_nodes->visited = true;// mark the neighboring node as visited.
		
	 } 
	 
	 
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.

RouteModel::Node *RoutePlanner::NextNode() {
	
	std::vector<float> f_value{}; // declase a vector of floats to capture the f values of the node.
	
	
	for (int i = 0; i < open_list.size(); i++) //loop through each node in the open list.
	{
		f_value.push_back = (*open_list[i]).h_value + (*open_list[i]).g_value; //get the f value of each node in the open list and add them in f_value vector.
	}
	
	int minElementIndex = std::min_element(f_value.begin(),f_value.end()) - f_value.begin(); //find the minimum f and its index. 	
	open_list.erase(open_list.begin() + minElementIndex); //remove that Node from the open_list.
	
	
	return open_list[minElementIndex];
	
	//std::sort(open_list.begin(), open_list.end())

}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;
    std::vector<RouteMode:: Node> nodeFound;
    // TODO: Implement your solution here.
	while(current_node->parent !=  nullptr) // if the node's parent is nullptr- means its a start node.
	{
		nodeFound = *current_node; // dereference the current node to get the Node.
		path_found.push_back(nodeFound); // store this nodeFound in path found. nodeFound will be the part of the path.
		
		distance = distance + (*current_node).distance(*(current_node->parent)); // get the distance between the current node and the parent node.
		current_node = current_node->parent; //store the address of the parent of the current node in current node, so that the previous node becomes 
		                                     //the current node.
	}
	//we have append the start node as well.
	path_found.push_back(*current_node);
	//reverse the path found so that the start node is at the beginning and the end node is at the end.
	reverse(path_found.begin(),path_found.end());
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    // TODO: Implement your solution here.
	start_node->visited = true; 
	open_list.push_back(start_node);
	
	while(!open_list.empty())
	{
		current_node = NextNode(); //captured the next node with minimum f value.
		
		if(currnet_node->distance(end_node) ==0)
		{
			m_model.path = ConstructFinalPath(current_node);
		}
		
		AddNeighbors(current_node);
	}

}