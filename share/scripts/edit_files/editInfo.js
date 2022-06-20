export default
{
                
        "config": {
                "coordinates": "euler-norm",        
                "FOV_theta" : 3.141592,
                "FOV_phi" : 1.570796,
                "max_roi" : 4,
                "tile_hor" : 3,
                "tile_vert" : 3,
                },
        "edit" : [
                        {
                        "frame" : 15,
                        "type": "instant",
                        "region_of_interest": [                                                                 
                                                {
                                                                "rank": 1,
                                                                "ROI_theta": 0.937,                      
                                                                "ROI_tiles": [3,4,5,6]
                                                },
                                                {       
                                                                "rank": 2,
                                                                "ROI_theta": 0.5,
                                                                "ROI_tiles": [6,7,8,9]
                                                }
                                                ]     
                        }
        ]
}

