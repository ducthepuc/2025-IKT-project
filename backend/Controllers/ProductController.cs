using Ecommerence.Core.Models;
using Ecommerce.Core.Helpers;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;

namespace EcommerenceApi.Controllers
{
    [Route("api/product")]
    [ApiController]
    public class ProductController : ControllerBase
    {
        [HttpGet("/GetAll")]
        public IActionResult GetAllProducts()
        {
            return Ok(new{message ="placeholder"});
        }

        [HttpGet("GetProduct/{id}")]
        public IActionResult GetProduct()
        {
            return Ok(new{message ="placeholder"});
        }
    }
}