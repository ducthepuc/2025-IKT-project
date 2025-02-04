using Ecommerence.Core.Models;
using Ecommerence.Core.Repositories;
using Microsoft.AspNetCore.Mvc;

namespace EcommerenceApi.Controllers
{

    [Route("api/[controller]")]
    [ApiController]
    public class HelloController : ControllerBase
    {
        [HttpGet]
        public IActionResult Hello()
        {
            return Ok(new { message = "Hello from the backend!" });
        }
    }
}
